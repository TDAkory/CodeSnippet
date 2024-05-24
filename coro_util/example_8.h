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

    T GetOrThrow() {
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

template <typename Result, typename Executor>
struct TaskAwaiter {
    explicit TaskAwaiter(coro::AbstractExecutor *executor, Task<Result, Executor> &&task) noexcept
        : executor_(executor), task_(std::move(task)) {}

    TaskAwaiter(TaskAwaiter &) = delete;
    TaskAwaiter &operator=(TaskAwaiter &) = delete;

    constexpr bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<> handle) noexcept {
        task_.finally([handle, this]() { executor_->execute([handle]() { handle.resume(); }); });
    }

    Result await_resume() noexcept { return task_.get_result(); }

 private:
    Task<Result, Executor> task_;
    coro::AbstractExecutor *executor_;
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

struct SleepAwaiter {
    explicit SleepAwaiter(coro::AbstractExecutor *executor, long long duration) noexcept
        : executor_(executor), duration_(duration) {}

    bool await_ready() const { return false; }

    void await_suspend(std::coroutine_handle<> handle) const {
        static coro::Scheduler scheduler;

        scheduler.execute([this, handle]() { executor_->execute([handle]() { handle.resume(); }); }, duration_);
    }

    void await_resume() {}

 private:
    coro::AbstractExecutor *executor_;
    long long duration_;
};

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
struct WriteAwaiter {
    Channel<ValueType> *channel_;
    coro::AbstractExecutor *executor_;
    ValueType value_;
    std::coroutine_handle<> handle_;

    WriteAwaiter(Channel<ValueType> *channel, ValueType value) : channel_(channel), value_(value) {}

    ~WriteAwaiter() {
        if (channel_) {
            channel_->remove_writer(this);
        }
    }

    WriteAwaiter(WriteAwaiter &&other) noexcept
        : channel_(std::exchange(other.channel_, nullptr)), executor_(std::exchange(other.executor_, nullptr)),
          value_(other.value_), handle_(other.handle_) {}

    bool await_ready() { return false; }

    auto await_suspend(std::coroutine_handle<> handle) {
        this->handle_ = handle;
        channel_->try_push_writer(this);
    }

    void await_resume() {
        channel_->check_closed();
        channel_ = nullptr;
    }

    void resume() {
        if (executor_) {
            executor_->execute([this]() { handle_.resume(); });
        } else {
            handle_.resume();
        }
    }
};

template <typename ValueType>
struct ReadAwaiter {
    Channel<ValueType> *channel_;
    coro::AbstractExecutor *executor_ = nullptr;
    ValueType value_;

    ValueType *p_value_ = nullptr;
    std::coroutine_handle<> handle_;

    explicit ReadAwaiter(Channel<ValueType> *channel) : channel_(channel) {}

    ~ReadAwaiter() {
        if (channel_) {
            channel_->remove_reader(this);
        }
    }

    ReadAwaiter(ReadAwaiter &&other) noexcept
        : channel_(std::exchange(other.channel_, nullptr)), executor_(std::exchange(other.executor_, nullptr)),
          value_(other.value_), p_value_(std::exchange(other.p_value_, nullptr)), handle_(other.handle_) {}

    bool await_ready() { return false; }

    auto await_suspend(std::coroutine_handle<> coroutine_handle) {
        this->handle_ = coroutine_handle;
        channel_->try_push_reader(this);
    }

    int await_resume() {
        channel_->check_closed();
        channel_ = nullptr;
        return value_;
    }

    void resume(ValueType value) {
        this->value_ = value;
        if (p_value_) {
            *p_value_ = value;
        }
        resume();
    }

    void resume() {
        if (executor_) {
            executor_->execute([this]() { handle_.resume(); });
        } else {
            handle_.resume();
        }
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
        return result_->GetOrThrow();
    }

    template <typename _ResultType, typename _Executor>
    TaskAwaiter<_ResultType, _Executor> await_transform(Task<_ResultType, _Executor> &&task) {
        return TaskAwaiter<_ResultType, _Executor>(&executor_, std::move(task));
    }

    template <typename _Rep, typename _Period>
    SleepAwaiter await_transform(std::chrono::duration<_Rep, _Period> &&duration) {
        return SleepAwaiter(&executor_, std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
    }

    template <typename _ValueType>
    auto await_transform(ReadAwaiter<_ValueType> read_awaiter) {
        read_awaiter.executor_ = &executor_;
        return read_awaiter;
    }

    template <typename _ValueType>
    auto await_tranform(WriteAwaiter<_ValueType> write_awaiter) {
        write_awaiter.executor_ = &executor_;
        return write_awaiter;
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

    template <typename _ResultType, typename _Executor>
    TaskAwaiter<_ResultType, _Executor> await_transform(Task<_ResultType, _Executor> &&task) {
        return TaskAwaiter<_ResultType, _Executor>(&executor_, std::move(task));
    }

    template <typename _Rep, typename _Period>
    SleepAwaiter await_transform(std::chrono::duration<_Rep, _Period> &&duration) {
        return SleepAwaiter(&executor_, std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
    }

    template <typename _ValueType>
    auto await_transform(ReadAwaiter<_ValueType> reader_awaiter) {
        reader_awaiter.executor_ = &executor_;
        return reader_awaiter;
    }

    template <typename _ValueType>
    auto await_transform(WriteAwaiter<_ValueType> writer_awaiter) {
        writer_awaiter.executor_ = &executor_;
        return writer_awaiter;
    }

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

    ResultType get_result() { return handle_.promise().get_result(); }

    Task &then(std::function<void(ResultType)> &&func) {
        handle_.promise().on_completed([func](auto result) {
            try {
                func(result.GetOrThrow());
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
                result.GetOrThrow();
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
