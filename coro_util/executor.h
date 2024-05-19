//
// Created by zhaojieyi on 2024/5/19.
//

#ifndef CODESNIPPET_EXECUTOR_H
#define CODESNIPPET_EXECUTOR_H

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

namespace coro {

class AbstractExecutor {
 public:
    virtual void execute(std::function<void()> &&func) = 0;
};

class NoopExecutor : public AbstractExecutor {
 public:
    void execute(std::function<void()> &&func) override { func(); }
};

class NewThreadExecutor : public AbstractExecutor {
 public:
    void execute(std::function<void()> &&func) override { std::thread(func).detach(); }
};

class AsyncExecutor : public AbstractExecutor {
 public:
    void execute(std::function<void()> &&func) override { auto future = std::async(func); }
};

class LooperExecutor : public AbstractExecutor {
 private:
    std::condition_variable queue_condition;
    std::mutex queue_lock;
    std::queue<std::function<void()>> executable_queue;

    // true 的时候是工作状态，如果要关闭事件循环，就置为 false
    std::atomic<bool> is_active;
    std::thread work_thread;

    // 处理事件循环
    void run_loop() {
        // 检查当前事件循环是否是工作状态，或者队列没有清空
        while (is_active.load(std::memory_order_relaxed) || !executable_queue.empty()) {
            std::unique_lock lock(queue_lock);
            if (executable_queue.empty()) {
                // 队列为空，需要等待新任务加入队列或者关闭事件循环的通知
                queue_condition.wait(lock);
                // 如果队列为空，那么说明收到的是关闭的通知
                if (executable_queue.empty()) {
                    // 现有逻辑下此处用 break 也可
                    // 使用 continue 可以再次检查状态和队列，方便将来扩展
                    continue;
                }
            }
            // 取出第一个任务，解锁再执行。
            // 解锁非常：func 是外部逻辑，不需要锁保护；func 当中可能请求锁，导致死锁
            auto func = executable_queue.front();
            executable_queue.pop();
            lock.unlock();

            func();
        }
    }

 public:
    LooperExecutor() {
        is_active.store(true, std::memory_order_relaxed);
        work_thread = std::thread(&LooperExecutor::run_loop, this);
    }

    ~LooperExecutor() {
        shutdown(false);
        // 等待线程执行完，防止出现意外情况
        join();
    }

    void execute(std::function<void()> &&func) override {
        std::unique_lock lock(queue_lock);
        if (is_active.load(std::memory_order_relaxed)) {
            executable_queue.push(func);
            lock.unlock();
            // 通知队列，主要用于队列之前为空时调用 wait 等待的情况
            // 通知不需要加锁，否则锁会交给 wait 方导致当前线程阻塞
            queue_condition.notify_one();
        }
    }

    void shutdown(bool wait_for_complete = true) {
        // 修改后立即生效，在 run_loop 当中就能尽早（加锁前）就检测到 is_active 的变化
        is_active.store(false, std::memory_order_relaxed);
        if (!wait_for_complete) {
            std::unique_lock lock(queue_lock);
            // 清空任务队列
            decltype(executable_queue) empty_queue;
            std::swap(executable_queue, empty_queue);
            lock.unlock();
        }

        // 通知 wait 函数，避免 Looper 线程不退出
        // 不需要加锁，否则锁会交给 wait 方导致当前线程阻塞
        queue_condition.notify_all();
    }

    void join() {
        if (work_thread.joinable()) {
            work_thread.join();
        }
    }
};

class SharedLooperExecutor : public AbstractExecutor {
 public:
    void execute(std::function<void()> &&func) override {
        static LooperExecutor sharedLooperExecutor;
        sharedLooperExecutor.execute(std::move(func));
    }
};

}  // namespace coro

#endif
