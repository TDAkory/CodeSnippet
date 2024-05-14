//
// Created by zhaojieyi on 2024/5/14.
//

#ifndef CODESNIPPET_EXAMPLE_1_H
#define CODESNIPPET_EXAMPLE_1_H

#include <chrono>
#include <coroutine>
#include <future>
#include <iostream>
#include <thread>

struct SimpleAwaiter {
    int value;
    // false挂起，true则不挂起
    bool await_ready() { return false; }
    // 挂起后 await_suspend 被调用，coroutine_handle 就是当前协程体的handle
    // 返回值 void 表示将执行权还给调用 or 恢复当前协程的函数
    void await_suspend(std::coroutine_handle<> coroutine_handle) {
        // 通过 async 执行异步执行
        (void)std::async([=]() {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
            coroutine_handle.resume();  // 恢复当前协程体
        });
    }
    // 协程恢复执行后，await_resume 被调用
    // 返回值不限定，其返回值就是 co_await 表达式的返回值
    int await_resume() { return value; }
};

struct Result {
    // 返回值内部定义的 promise_type
    struct promise_type {
        // 协程体执行第一步，可用于调度
        std::suspend_never initial_suspend() { return std::suspend_never(); }
        // 协程体执行最后一步，可用于清理
        std::suspend_never final_suspend() noexcept { return std::suspend_never(); }
        // Result 的构建方法，初始化协程上下文后，会调用它立刻创建 协程函数 的返回值
        Result get_return_object() { return {}; }
        // 若协程体执行抛出异常，由该函数处理
        void unhandled_exception() {}
        // 未定义 return_value，说明这个协程体没有返回值
        // ??? return_value() { ... }
    };
};

// 协程函数，返回值类型满足 coroutine_traits 约束
Result Coroutine() {
    std::cout << 1 << std::endl;
    // 按照定义，立即挂起 --> 等待 1s 后恢复
    std::cout << co_await SimpleAwaiter{.value = 1000} << std::endl;
    std::cout << 2 << std::endl;
}

// 执行输出
// 1
// 1000
// 2

#endif  // CODESNIPPET_EXAMPLE_1_H
