#pragma once

#include <exception>
namespace coro {

template <typename RetType>
struct TaskResult {
    explicit TaskResult() = default;
    explicit TaskResult(RetType &&value) : v_(value) {}
    explicit TaskResult(std::exception_ptr &&e_ptr) : ex_(e_ptr) {}

    TaskResult(const TaskResult &) = default;
    TaskResult &operator=(const TaskResult &) = default;

    TaskResult(TaskResult &&other) : v_(std::move(other.v_)), ex_(std::move(other.ex_)) {
        other.v_ = RetType();
        other.ex_ = nullptr;
    }

    TaskResult &operator=(TaskResult &&other) {
        v_ = std::move(other.v_);
        ex_ = std::move(other.ex_);
        other.v_ = RetType();
        other.ex_ = nullptr;
    }

    RetType GetOrThrow() {
        if (ex_) {
            std::rethrow_exception(ex_);
        }
        return v_;
    }

    RetType v_{};
    std::exception_ptr ex_{nullptr};
};

template <>
struct TaskResult<void> {
    explicit TaskResult() = default;
    explicit TaskResult(std::exception_ptr &&e_ptr) : ex_(e_ptr) {}

    TaskResult(const TaskResult &) = default;
    TaskResult &operator=(const TaskResult &) = default;

    TaskResult(TaskResult &&other) : ex_(std::move(other.ex_)) { other.ex_ = nullptr; }

    TaskResult &operator=(TaskResult &&other) {
        ex_ = std::move(other.ex_);
        other.ex_ = nullptr;
    }

    void GetOrThrow() {
        if (ex_) {
            std::rethrow_exception(ex_);
        }
    }

    std::exception_ptr ex_{nullptr};
};

}  // namespace coro