//
// Created by zhaojieyi on 2024/5/15.
//

#ifndef CODESNIPPET_EXAMPLE_3_H
#define CODESNIPPET_EXAMPLE_3_H

#include <coroutine>
#include <exception>
#include <functional>
#include <initializer_list>
#include <list>
#include <utility>

namespace example_3 {

template <typename T>
struct Generator {
    class ExhaustedException : std::exception {};
    struct promise_type {
        T value;
        bool is_ready{false};

        std::suspend_always initial_suspend() { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        void unhandled_exception() {}

        Generator get_return_object() { return Generator(std::coroutine_handle<promise_type>::from_promise(*this)); }

        void return_void() {}

        std::suspend_always await_transfrom(T value) {
            this->value = value;
            return {};
        }

        std::suspend_always yield_value(T value) {
            this->value = value;
            is_ready = true;
            return {};
        }
    };

    explicit Generator(std::coroutine_handle<promise_type> handle) noexcept : handle(handle) {}

    Generator(Generator &&generator) noexcept : handle(std::exchange(generator.handle, {})) {}

    Generator(Generator &) = delete;
    Generator &operator=(Generator &) = delete;

    ~Generator() {
        if (handle) {
            handle.destroy();
        }
    }

    bool has_next() {
        if (handle.done()) {
            return false;
        }
        if (!handle.promise().is_ready) {
            handle.resume();
        }

        if (handle.done()) {
            return false;
        } else {
            return true;
        }
    }

    int next() {
        if (has_next()) {
            handle.promise().is_ready = false;
            return handle.promise().value;
        }
        throw ExhaustedException();
    }

    Generator static FromArray(T array[], int n) {
        for (int i = 0; i < n; ++i) {
            co_yield array[i];
        }
    }

    Generator static FromList(std::list<T> list) {
        for (auto t : list) {
            co_yield t;
        }
    }

    Generator static From(std::initializer_list<T> args) {
        for (auto t : args) {
            co_yield t;
        }
    }

    template <typename... TArgs>
    Generator static From(TArgs... args) {
        (co_yield args, ...);
    }

    template <typename U>
    Generator<U> Map(std::function<U(T)> f) {
        while (has_next()) {
            co_yield f(next());
        }
    }

    template <typename F>
    Generator<std::invoke_result_t<F, T>> Map2(F f) {
        while (has_next()) {
            co_yield f(next());
        }
    }

    template <typename F>
    std::invoke_result_t<F, T> FlatMap(F f) {
        while (has_next()) {
            auto generator = f(next());
            while (generator.has_next()) {
                co_yield generator.next();
            }
        }
    }

    template <typename F>
    void ForEach(F f) {
        while (has_next()) {
            f(next());
        }
    }

    template <typename R, typename F>
    R Fold(R init, F f) {
        R ret = init;
        while (has_next()) {
            ret = f(ret, next());
        }
        return ret;
    }

    T Sum() {
        T sum = 0;
        while (has_next()) {
            sum += next();
        }
        return sum;
    }

    template <typename F>
    Generator Filter(F f) {
        while (has_next()) {
            T value = next();
            if (f(value)) {
                co_yield value;
            }
        }
    }

    Generator Take(int n) {
        int i = 0;
        while (has_next() && i++ < n) {
            co_yield next();
        }
    }

    template <typename F>
    Generator TakeWhile(F f) {
        while (has_next()) {
            T value = next();
            if (f(value)) {
                co_yield value;
            } else {
                break;
            }
        }
    }

    std::coroutine_handle<promise_type> handle;
};

Generator<int> Fibonacci() {
    co_yield 0;
    co_yield 1;

    int a = 0;
    int b = 1;
    while (true) {
        co_yield a + b;
        b = a + b;
        a = b - a;
    }
}

}  // namespace example_3
#endif  // CODESNIPPET_EXAMPLE_3_H
