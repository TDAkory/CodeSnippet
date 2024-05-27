//
// Created by zhaojieyi on 2024/5/23.
//

#ifndef CODESNIPPET_EXAMPLE_8_H
#define CODESNIPPET_EXAMPLE_8_H

#include <chrono>
#include <condition_variable>
#include <coroutine>
#include <exception>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <ratio>
#include <utility>
#include "executor.h"

namespace example_8 {

template <typename Result, typename Executor>
struct Task;

template <typename T>
struct Result {
    explicit Result() = default;
    explicit Result(T &&value) : value_(value) {}
    explicit Result(std::exception_ptr &&e_ptr) : exception_ptr_(e_ptr) {}

    T get_or_throw() {
        if (exception_ptr_) {
            std::rethrow_exception(exception_ptr_);
        }
        return value_;
    }

    T value_{};
    std::exception_ptr exception_ptr_{nullptr};
};

template <>
struct Result<void> {
    explicit Result() = default;

    explicit Result(std::exception_ptr &&exception_ptr) : _exception_ptr(exception_ptr) {}

    void get_or_throw() {
        if (_exception_ptr) {
            std::rethrow_exception(_exception_ptr);
        }
    }

 private:
    std::exception_ptr _exception_ptr;
};

template <typename R>
struct Awaiter {
    using ResultType = R;

    bool await_ready() const { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
        this->handle_ = handle;
        after_suspend();
    }

    R await_resume() {
        before_resume();
        return result_->get_or_throw();
    }

    void install_executor(coro::AbstractExecutor *executor) { executor_ = executor; }

    void resume(R value) {
        dispatch([this, value]() {
            result_ = Result<R>(static_cast<R>(value));
            handle_.resume();
        });
    }

    void resume_unsafe() {
        dispatch([this]() { handle_.resume(); });
    }

    void resume_exception(std::exception_ptr &ex) {
        dispatch([this, ex]() {
            result_ = Result<R>(static_cast<std::exception_ptr>(ex));
            handle_.resume();
        });
    }

 protected:
    std::optional<Result<R>> result_{};

    virtual void after_suspend() {}

    virtual void before_resume() {}

 private:
    coro::AbstractExecutor *executor_{nullptr};
    std::coroutine_handle<> handle_{};

    void dispatch(std::function<void()> &&f) {
        if (executor_) {
            executor_->execute(std::move(f));
        } else {
            f();
        }
    }
};

template <>
struct Awaiter<void> {
    using ResultType = void;

    bool await_ready() const { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
        this->handle_ = handle;
        after_suspend();
    }

    void await_resume() {
        before_resume();
        return result_->get_or_throw();
    }

    void install_executor(coro::AbstractExecutor *executor) { executor_ = executor; }

    void resume() {
        dispatch([this]() { handle_.resume(); });
    }

    void resume_unsafe() {
        dispatch([this]() { handle_.resume(); });
    }

    void resume_exception(std::exception_ptr &ex) {
        dispatch([this, ex]() {
            result_ = Result<void>(static_cast<std::exception_ptr>(ex));
            handle_.resume();
        });
    }

 protected:
    std::optional<Result<void>> result_{};

    virtual void after_suspend() {}

    virtual void before_resume() {}

 private:
    coro::AbstractExecutor *executor_{nullptr};
    std::coroutine_handle<> handle_{};

    void dispatch(std::function<void()> &&f) {
        if (executor_) {
            executor_->execute(std::move(f));
        } else {
            f();
        }
    }
};

template <typename Result, typename Executor>
struct TaskAwaiter : public Awaiter<Result> {
    explicit TaskAwaiter(Task<Result, Executor> &&task) noexcept : task_(std::move(task)) {}

    TaskAwaiter(TaskAwaiter &) = delete;
    TaskAwaiter &operator=(TaskAwaiter &) = delete;

    TaskAwaiter(TaskAwaiter &&awaiter) noexcept : Awaiter<Result>(awaiter), task_(std::move(awaiter.task_)) {}

 protected:
    void after_suspend() override {
        task_.finally([this]() { this->resume_unsafe(); });
    }

    void before_resume() override { this->result_ = Result(task_.get_result()); }

 private:
    Task<Result, Executor> task_;
};

struct DispatchAwaiter {
    explicit DispatchAwaiter(coro::AbstractExecutor *executor) noexcept : executor_(executor) {}

    bool await_ready() const { return false; }

    void await_suspend(std::coroutine_handle<> handle) const {
        executor_->execute([handle]() { handle.resume(); });
    }

    void await_resume() {}

 private:
    coro::AbstractExecutor *executor_;
};

struct SleepAwaiter : Awaiter<void> {
    explicit SleepAwaiter(long long duration) noexcept : duration_(duration) {}

    template <typename _Rep, typename _Period>
    explicit SleepAwaiter(std::chrono::duration<_Rep, _Period> &&duration) noexcept
        : duration_(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()) {}

    void after_suspend() override {
        static coro::Scheduler scheduler;
        scheduler.execute([this] { resume(); }, duration_);
    }

 private:
    long long duration_;
};

template <typename R>
struct FutureAwaiter : public Awaiter<R> {
    explicit FutureAwaiter(std::future<R> &&future) noexcept : future_(std::move(future)) {}

    FutureAwaiter(FutureAwaiter &&awaiter) noexcept : Awaiter<R>(awaiter), future_(std::move(awaiter._future)) {}

    FutureAwaiter(FutureAwaiter &) = delete;

    FutureAwaiter &operator=(FutureAwaiter &) = delete;

 protected:
    void after_suspend() override {
        // std::future::get 会阻塞等待结果的返回，因此我们新起一个线程等待结果的返回
        // 如果后续 std::future 增加了回调，这里直接注册回调即可
        std::thread([this]() {
            // 获取结果，并恢复协程
            this->resume(this->future_.get());
        }).detach();
        // std::thread 必须 detach 或者 join 二选一
        // 也可以使用 std::jthread
    }

 private:
    std::future<R> future_;
};

template <typename R>
FutureAwaiter<R> as_awaiter(std::future<R> &&future) {
    return FutureAwaiter(std::move(future));
}

template <typename ValueType>
struct WriteAwaiter;

template <typename ValueType>
struct ReadAwaiter;

template <typename ValueType>
struct Channel {
    struct ChannelClosedException : public std::exception {
        const char *what() const noexcept override { return "Channel is closed."; }
    };

    void check_closed() {
        if (!active_) {
            throw ChannelClosedException();
        }
    }

    explicit Channel(int cap = 0) : buf_cap_(cap) { active_.store(true); }

    bool is_active() { return active_.load(); }

    void close() {
        bool expect = true;
        if (active_.compare_exchange_strong(expect, false, std::memory_order_relaxed)) {
            clean_up();
        }
    }

    Channel(Channel &&channel) = delete;
    Channel(Channel &) = delete;
    Channel &operator=(Channel &) = delete;

    ~Channel() { close(); }

    auto write(ValueType value) {
        check_closed();
        return WriteAwaiter<ValueType>(this, value);
    }

    auto operator<<(ValueType value) { return write(value); }

    auto read() {
        check_closed();
        return ReadAwaiter<ValueType>(this);
    }

    auto operator>>(ValueType &value_ref) {
        auto awaiter = read();
        awaiter.p_value_ = &value_ref;
        return awaiter;
    }

    void try_push_writer(WriteAwaiter<ValueType> *writer) {
        std::unique_lock lk(mut_);
        check_closed();
        if (!read_list_.empty()) {
            auto reader = read_list_.front();
            read_list_.pop_front();
            lk.unlock();

            reader->resume(writer->value_);
            writer->resume();
            return;
        }
        if (buffer_.size() < buf_cap_) {
            buffer_.push(writer->value_);
            lk.unlock();
            writer->resume();
            return;
        }
        write_list_.push_back(writer);
    }

    void try_push_reader(ReadAwaiter<ValueType> *reader) {
        std::unique_lock lk(mut_);
        check_closed();
        if (!buffer_.empty()) {
            auto value = buffer_.front();
            buffer_.pop();

            if (!write_list_.empty()) {
                auto writer = write_list_.front();
                write_list_.pop_front();
                buffer_.push(writer->value_);
                lk.unlock();

                writer->resume();
            } else {
                lk.unlock();
            }

            reader->resume(value);
            return;
        }
        if (!write_list_.empty()) {
            auto writer = write_list_.front();
            write_list_.pop_front();
            lk.unlock();

            reader->resume(writer->value_);
            writer->resume();
            return;
        }
        read_list_.push_back(reader);
    }

    void remove_reader(ReadAwaiter<ValueType> *reader) {
        std::lock_guard lk(mut_);
        read_list_.remove(reader);
    }

    void remove_writer(WriteAwaiter<ValueType> *writer) {
        std::lock_guard lk(mut_);
        write_list_.remove(writer);
    }

 private:
    void clean_up() {
        std::lock_guard lk(mut_);
        for (auto write : write_list_) {
            write->resume();
        }
        write_list_.clear();
        for (auto read : read_list_) {
            read->resume();
        }
        read_list_.clear();

        decltype(buffer_) empty_buffer;
        std::swap(buffer_, empty_buffer);
    }

 private:
    int buf_cap_;
    std::queue<ValueType> buffer_;
    std::list<WriteAwaiter<ValueType> *> write_list_;
    std::list<ReadAwaiter<ValueType> *> read_list_;

    std::atomic_bool active_{false};
    std::mutex mut_;
    std::condition_variable cond_;
};

template <typename ValueType>
struct WriteAwaiter : public Awaiter<void> {
    Channel<ValueType> *channel_;
    ValueType value_;

    WriteAwaiter(Channel<ValueType> *channel, ValueType value) : channel_(channel), value_(value) {}

    ~WriteAwaiter() {
        if (channel_) {
            channel_->remove_writer(this);
        }
    }

    WriteAwaiter(WriteAwaiter &&other) noexcept
        : Awaiter(other), channel_(std::exchange(other.channel_, nullptr)), value_(other.value_) {}

    void after_suspend() override { channel_->try_push_writer(this); }

    void before_resume() override {
        channel_->check_closed();
        channel_ = nullptr;
    }
};

template <typename ValueType>
struct ReadAwaiter : public Awaiter<ValueType> {
    Channel<ValueType> *channel_;
    ValueType *p_value_ = nullptr;

    explicit ReadAwaiter(Channel<ValueType> *channel) : Awaiter<ValueType>(), channel_(channel) {}

    ~ReadAwaiter() {
        if (channel_) {
            channel_->remove_reader(this);
        }
    }

    ReadAwaiter(ReadAwaiter &&other) noexcept
        : Awaiter<ValueType>(other), channel_(std::exchange(other.channel_, nullptr)),
          p_value_(std::exchange(other.p_value_, nullptr)) {}

    void after_suspend() override { channel_->try_push_reader(this); }

    void before_resume() override {
        channel_->check_closed();
        if (p_value_) {
            *p_value_ = this->result_->get_or_throw();
        }
        channel_ = nullptr;
    }
};

template <typename ResultType, typename Executor>
struct TaskPromise {
    DispatchAwaiter initial_suspend() { return DispatchAwaiter{&executor_}; }

    std::suspend_always final_suspend() noexcept { return {}; }

    Task<ResultType, Executor> get_return_object() {
        return Task{std::coroutine_handle<TaskPromise>::from_promise(*this)};
    }

    void unhandled_exception() {
        std::lock_guard<std::mutex> lk(mut_);
        result_ = Result<ResultType>(std::current_exception());
        cond_.notify_all();
        notify_callbacks();
    }

    void return_value(ResultType value) {
        std::lock_guard<std::mutex> lk(mut_);
        result_ = Result<ResultType>(std::move(value));
        cond_.notify_all();
        notify_callbacks();
    }

    void on_completed(std::function<void(Result<ResultType>)> &&func) {
        std::unique_lock<std::mutex> lk(mut_);
        if (result_.has_value()) {
            auto value = result_.value();
            lk.unlock();
            func(value);
        } else {
            callbacks_.push_back(func);
        }
    }

    ResultType get_result() {
        std::unique_lock<std::mutex> lk(mut_);
        if (!result_.has_value()) {
            cond_.wait(lk);
        }
        return result_->get_or_throw();
    }

 private:
    void notify_callbacks() {
        auto value = result_.value();
        for (auto &callback : callbacks_) {
            callback(value);
        }
        callbacks_.clear();
    }

 private:
    std::optional<Result<ResultType>> result_;

    std::mutex mut_;
    std::condition_variable cond_;

    std::list<std::function<void(Result<ResultType>)>> callbacks_;

    Executor executor_;
};

template <typename Executor>
struct TaskPromise<void, Executor> {
    DispatchAwaiter initial_suspend() { return DispatchAwaiter{&executor_}; }

    std::suspend_always final_suspend() noexcept { return {}; }

    Task<void, Executor> get_return_object() { return Task{std::coroutine_handle<TaskPromise>::from_promise(*this)}; }

    void get_result() {
        // blocking for result or throw on exception
        std::unique_lock lock(completion_lock);
        if (!result.has_value()) {
            completion.wait(lock);
        }
        result->get_or_throw();
    }

    void unhandled_exception() {
        std::lock_guard lock(completion_lock);
        result = Result<void>(std::current_exception());
        completion.notify_all();
        notify_callbacks();
    }

    void return_void() {
        std::lock_guard lock(completion_lock);
        result = Result<void>();
        completion.notify_all();
        notify_callbacks();
    }

    void on_completed(std::function<void(Result<void>)> &&func) {
        std::unique_lock lock(completion_lock);
        if (result.has_value()) {
            auto value = result.value();
            lock.unlock();
            func(value);
        } else {
            completion_callbacks.push_back(func);
        }
    }

 private:
    std::optional<Result<void>> result;

    std::mutex completion_lock;
    std::condition_variable completion;

    std::list<std::function<void(Result<void>)>> completion_callbacks;

    Executor executor_;

    void notify_callbacks() {
        auto value = result.value();
        for (auto &callback : completion_callbacks) {
            callback(value);
        }
        completion_callbacks.clear();
    }
};

template <typename ResultType, typename Executor = coro::NewThreadExecutor>
struct Task {
    using promise_type = TaskPromise<ResultType, Executor>;

    auto as_awaiter() { return TaskAwaiter<ResultType, Executor>(std::move(*this)); }

    ResultType get_result() { return handle_.promise().get_result(); }

    Task &then(std::function<void(ResultType)> &&func) {
        handle_.promise().on_completed([func](auto result) {
            try {
                func(result.get_or_throw());
            }
            catch (std::exception &e) {
            }
        });
        return *this;
    }

    Task &catching(std::function<void(std::exception &)> &&func) {
        handle_.promise().on_completed([func](auto result) {
            try {
                // 忽略返回值
                result.get_or_throw();
            }
            catch (std::exception &e) {
                func(e);
            }
        });
        return *this;
    }

    Task &finally(std::function<void()> &&func) {
        handle_.promise().on_completed([func](auto result) { func(); });
        return *this;
    }

    explicit Task(std::coroutine_handle<promise_type> handle) noexcept : handle_(handle) {}

    Task(Task &&task) noexcept : handle_(std::exchange(task.handle_, {})) {}

    Task(Task &) = delete;

    Task &operator=(Task &) = delete;

    ~Task() {
        if (handle_) {
            handle_.destroy();
        }
    }

 private:
    std::coroutine_handle<promise_type> handle_;
};

template <typename Executor>
struct Task<void, Executor> {
    using promise_type = TaskPromise<void, Executor>;

    void get_result() { handle_.promise().get_result(); }

    Task &then(std::function<void()> &&func) {
        handle_.promise().on_completed([func](auto result) {
            try {
                result.get_or_throw();
                func();
            }
            catch (std::exception &e) {
                // ignore.
            }
        });
        return *this;
    }

    Task &catching(std::function<void(std::exception &)> &&func) {
        handle_.promise().on_completed([func](auto result) {
            try {
                result.get_or_throw();
            }
            catch (std::exception &e) {
                func(e);
            }
        });
        return *this;
    }

    Task &finally(std::function<void()> &&func) {
        handle_.promise().on_completed([func](auto result) { func(); });
        return *this;
    }

    explicit Task(std::coroutine_handle<promise_type> handle) noexcept : handle_(handle) {}

    Task(Task &&task) noexcept : handle_(std::exchange(task.handle_, {})) {}

    Task(Task &) = delete;

    Task &operator=(Task &) = delete;

    ~Task() {
        if (handle_)
            handle_.destroy();
    }

 private:
    std::coroutine_handle<promise_type> handle_;
};

}  // namespace example_8

#endif  // CODESNIPPET_EXAMPLE_7_H
