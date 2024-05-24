//
// Created by zhaojieyi on 2024/5/14.
//
#include <algorithm>
#include "../util/simple_log.h"
#include "example_1.h"
#include "example_2.h"
#include "example_3.h"
#include "example_4.h"
#include "example_5.h"
#include "example_6.h"
#include "example_7.h"

using namespace std::chrono_literals;

example_4::Task<int> simple_task2() {
    DEBUG("task 2 start ...");
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    DEBUG("task 2 returns after 1s.");
    co_return 2;
}

example_4::Task<int> simple_task3() {
    DEBUG("in task 3 start ...");
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    DEBUG("task 3 returns after 2s.");
    co_return 3;
}

example_4::Task<int> simple_task() {
    DEBUG("task start ...");
    auto result2 = co_await simple_task2();
    DEBUG("returns from task2: ", result2);
    auto result3 = co_await simple_task3();
    DEBUG("returns from task3: ", result3);
    co_return 1 + result2 + result3;
}

example_5::Task<int, coro::AsyncExecutor> ex_simple_task2() {
    DEBUG("task 2 start ...");
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    DEBUG("task 2 returns after 1s.");
    co_return 2;
}

// 使用了 NewThread 调度器
// 这意味着每个恢复的位置都会新建一个线程来执行
example_5::Task<int, coro::NewThreadExecutor> ex_simple_task3() {
    DEBUG("in task 3 start ...");
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    DEBUG("task 3 returns after 2s.");
    co_return 3;
}

// 使用了 Looper 调度器
// 这意味着每个恢复的位置都会在同一个线程上执行
example_5::Task<int, coro::LooperExecutor> ex_simple_task() {
    DEBUG("task start ...");
    auto result2 = co_await ex_simple_task2();
    DEBUG("returns from task2: ", result2);
    auto result3 = co_await ex_simple_task3();
    DEBUG("returns from task3: ", result3);
    co_return 1 + result2 + result3;
}

example_6::Task<int, coro::AsyncExecutor> delay_simple_task2() {
    DEBUG("task 2 start ...");
    co_await 1s;
    DEBUG("task 2 returns after 1s.");
    co_return 2;
}

example_6::Task<int, coro::NewThreadExecutor> delay_simple_task3() {
    DEBUG("in task 3 start ...");
    co_await 2s;
    DEBUG("task 3 returns after 2s.");
    co_return 3;
}

example_6::Task<int, coro::LooperExecutor> delay_simple_task() {
    DEBUG("task start ...");
    co_await 100ms;
    DEBUG("after 100ms ...");
    auto result2 = co_await delay_simple_task2();
    DEBUG("returns from task2: ", result2);

    co_await 500ms;
    DEBUG("after 500ms ...");
    auto result3 = co_await delay_simple_task3();
    DEBUG("returns from task3: ", result3);
    co_return 1 + result2 + result3;
}

example_7::Task<void, coro::LooperExecutor> Producer(example_7::Channel<int> &channel) {
    int i = 0;
    while (i < 10) {
        DEBUG("send: ", i);
        co_await(channel << i++);
        co_await 300ms;
    }
    channel.close();
    DEBUG("close channel, exit.");
}

example_7::Task<void, coro::LooperExecutor> Consumer(example_7::Channel<int> &channel) {
    while (channel.is_active()) {
        try {
            // 或者使用 read 函数：auto received = co_await channel.read();
            int received;
            co_await(channel >> received);
            DEBUG("receive: ", received);
            co_await 2s;
        }
        catch (std::exception &e) {
            DEBUG("exception: ", e.what());
        }
    }

    DEBUG("exit.");
}

example_7::Task<void, coro::LooperExecutor> Consumer2(example_7::Channel<int> &channel) {
    while (channel.is_active()) {
        try {
            auto received = co_await channel.read();
            DEBUG("receive2: ", received);
            co_await 3s;
        }
        catch (std::exception &e) {
            DEBUG("exception2: ", e.what());
        }
    }

    DEBUG("exit.");
}

int main() {
    //    {
    //        using namespace example_1;
    //        Coroutine();  // basic
    //    }
    //    {
    //        using namespace example_2;
    //        DoGenerate();     //  Generator
    //        DoFibonacci(10);  //  Fibonacci
    //    }

    {
        using namespace example_3;
        //        Generator<std::string> g_str = Fibonacci().Map<std::string>([](int i) { return std::to_string(i); });
        //        Generator<std::string> g_str_2 = Fibonacci().Map2([](int i) { return std::to_string(i); });
        //
        //        Generator<int>::From(1, 2, 3, 4)
        //            .FlatMap([](auto i) -> Generator<int> {
        //                for (int j = 0; j < i; j++) {
        //                    co_yield j;
        //                }
        //            })
        //            .ForEach([](auto i) {
        //                if (0 == i) {
        //                    std::cout << std::endl;
        //                }
        //                std::cout << "*";
        //            });
        //
        //        auto result = Generator<int>::From(1, 2, 3, 4, 5, 6).fold(1, [](auto acc, auto i) {
        //            return acc * i;  // 计算阶乘
        //        });
        //        std::cout << result << std::endl;
        //
        //        Generator<int>::From(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
        //            .Filter([](auto i) {
        //                std::cout << "Filter: " << i << std::endl;
        //                return i % 2 == 0;
        //            })
        //            .Map2([](auto i) {
        //                std::cout << "map: " << i << std::endl;
        //                return i * 3;
        //            })
        //            .FlatMap([](auto i) -> Generator<int> {
        //                std::cout << "flat_map: " << i << std::endl;
        //                for (int j = 0; j < i; ++j) {
        //                    co_yield j;
        //                }
        //            })
        //            .Take(10)
        //            .ForEach([](auto i) { std::cout << "for_each: " << i << std::endl; });
    }

    {
        using namespace example_4;

        // auto simpleTask = simple_task();
        // simpleTask.then([](int i) { DEBUG("simple task end: ", i); }).catching([](std::exception &e) {
        //     DEBUG("error occurred ", e.what());
        // });
        // try {
        //     auto i = simpleTask.get_result();
        //     DEBUG("simple task end from get: ", i);
        // }
        // catch (std::exception &e) {
        //     DEBUG("error: ", e.what());
        // }
    }
    {
        //        using namespace example_5;
        //        auto simpleTask = ex_simple_task();
        //        simpleTask.then([](int i) { DEBUG("simple task end: ", i); }).catching([](std::exception &e) {
        //            DEBUG("error occurred", e.what());
        //        });
        //        try {
        //            auto i = simpleTask.get_result();
        //            DEBUG("simple task end from get: ", i);
        //        }
        //        catch (std::exception &e) {
        //            DEBUG("error: ", e.what());
        //        }
    }
    {
        using namespace example_6;
        //        auto schedular = coro::Scheduler();
        //        DEBUG("start");
        //
        //        schedular.execute([]() { DEBUG("2"); }, 100);
        //        schedular.execute([]() { DEBUG("1"); }, 50);
        //        schedular.execute([]() { DEBUG("6"); }, 1000);
        //        schedular.execute([]() { DEBUG("5"); }, 500);
        //        schedular.execute([]() { DEBUG("3"); }, 200);
        //        schedular.execute([]() { DEBUG("4"); }, 300);
        //
        //        schedular.shutdown();
        //        schedular.join();
        //
        //        auto simpleTask = delay_simple_task();
        //        simpleTask.then([](int i) { DEBUG("simple task end: ", i); }).catching([](std::exception &e) {
        //            DEBUG("error occurred", e.what());
        //        });
        //        try {
        //            auto i = simpleTask.get_result();
        //            DEBUG("simple task end from get: ", i);
        //        }
        //        catch (std::exception &e) {
        //            DEBUG("error: ", e.what());
        //        }
    }
    {
        using namespace example_7;

        auto channel = Channel<int>(2);
        auto producer = Producer(channel);
        auto consumer = Consumer(channel);
        auto consumer2 = Consumer2(channel);

        // 等待协程执行完成再退出
        producer.get_result();
        consumer.get_result();
        consumer2.get_result();

        return 0;
    }

    return 0;
}