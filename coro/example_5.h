//
// Created by zhaojieyi on 2024/5/19.
//

#ifndef CODESNIPPET_EXAMPLE_5_H
#define CODESNIPPET_EXAMPLE_5_H

#include <condition_variable>
#include <coroutine>
#include <exception>
#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <utility>
#include "executor.h"

namespace example_5 {

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

    template <typename _ResultType, typename _Executor>
    TaskAwaiter<_ResultType, _Executor> await_transform(Task<_ResultType, _Executor> &&task) {
        return TaskAwaiter<_ResultType, _Executor>(&executor_, std::move(task));
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

template <typename ResultType, typename Executor = coro::NewThreadExecutor>
struct Task {
    using promise_type = TaskPromise<ResultType, Executor>;

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
}  // namespace example_5

#endif
