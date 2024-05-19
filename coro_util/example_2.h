//
// Created by zhaojieyi on 2024/5/15.
//

#ifndef CODESNIPPET_EXAMPLE_2_H
#define CODESNIPPET_EXAMPLE_2_H

#include <coroutine>
#include <iostream>
#include <utility>

namespace example_2 {

struct Generator {
    class ExhaustedException : std::exception {};

    struct promise_type {
        std::suspend_always initial_suspend() { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        void unhandled_exception() {}

        Generator get_return_object() { return Generator(std::coroutine_handle<promise_type>::from_promise(*this)); }

        void return_void() {}

        std::suspend_always await_transfrom(int value) {
            this->value = value;
            return {};
        }

        std::suspend_always yield_value(int value) {
            this->value = value;
            is_ready = true;
            return {};
        }

        int value;
        bool is_ready = false;
    };

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

    explicit Generator(std::coroutine_handle<promise_type> handle) noexcept : handle(handle) {}

    Generator(Generator &&generator) noexcept : handle(std::exchange(generator.handle, {})) {}

    Generator(Generator &) = delete;
    Generator &operator=(Generator &) = delete;

    ~Generator() {
        if (handle) {
            handle.destroy();
        }
    }

    std::coroutine_handle<promise_type> handle;
};

Generator Sequence() {
    int i = 0;
    while (i < 5) {
        // co_await i__;    // co_await promise.await_transform(expr)
        co_yield i++;  // co_yield ==  co_await promise.yield_value(expr)
    }
}

void DoGenerate() {
    Generator g = Sequence();
    for (int i = 0; i < 10; i++) {
        if (g.has_next()) {
            std::cout << g.next() << std::endl;
        } else {
            break;
        }
    }
}

Generator Fibonacci() {
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

void DoFibonacci(int N) {
    Generator g = Fibonacci();
    for (int i = 0; i < N; i++) {
        if (g.has_next()) {
            std::cout << g.next() << std::endl;
        } else {
            break;
        }
    }
}

}  // namespace example_2

#endif  // CODESNIPPET_EXAMPLE_2_H
