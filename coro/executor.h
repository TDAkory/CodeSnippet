//
// Created by zhaojieyi on 2024/5/19.
//

#ifndef CODESNIPPET_EXECUTOR_H
#define CODESNIPPET_EXECUTOR_H

#include <concepts>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

namespace coro {

template <class T>
concept Executable = requires(T a) {
    a.execute(std::function<void()>());
};

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

class DelayedExecutable {
 public:
    DelayedExecutable() = default;
    DelayedExecutable(std::function<void()> &&func, long long delay) : func_(func) {
        using namespace std;
        using namespace std::chrono;

        auto now = system_clock::now();
        auto current = duration_cast<milliseconds>(now.time_since_epoch()).count();

        scheduled_time_ = current + delay;
    }

    long long delay() const {
        using namespace std;
        using namespace std::chrono;

        auto now = system_clock::now();
        auto current = duration_cast<milliseconds>(now.time_since_epoch()).count();

        return scheduled_time_ - current;
    }

    long long get_scheduled_time() const { return scheduled_time_; }

    void operator()() { func_(); }

 private:
    long long scheduled_time_;
    std::function<void()> func_;
};

class DelayedExecutableCompare {
 public:
    bool operator()(DelayedExecutable &left, DelayedExecutable &right) {
        return left.get_scheduled_time() > right.get_scheduled_time();
    }
};

class Scheduler {
 private:
    std::condition_variable queue_cond_;
    std::mutex mut_;

    std::priority_queue<DelayedExecutable, std::vector<DelayedExecutable>, DelayedExecutableCompare> queue_;

    std::atomic_bool active_;
    std::thread worker_;

    void loop() {
        while (active_.load(std::memory_order_acquire) || !queue_.empty()) {
            DelayedExecutable executable;
            {
                std::unique_lock lk(mut_);
                if (queue_.empty()) {
                    queue_cond_.wait(lk, [&]() { return !active_ || !queue_.empty(); });
                    if (queue_.empty()) {
                        continue;
                    }
                }
                executable = queue_.top();
                long long delay = executable.delay();

                if (delay > 0) {
                    auto status = queue_cond_.wait_for(lk, std::chrono::milliseconds(delay));
                    if (status != std::cv_status::timeout) {
                        continue;
                    }
                }

                queue_.pop();
            }
            executable();
        }
    }

 public:
    Scheduler() {
        active_.store(true, std::memory_order_relaxed);
        worker_ = std::thread(&Scheduler::loop, this);
    }

    ~Scheduler() {
        shutdown(false);
        // 等待线程执行完，防止出现意外情况
        join();
    }

    void execute(std::function<void()> &&func, long long delay) {
        delay = delay < 0 ? 0 : delay;
        std::unique_lock lk(mut_);
        if (active_.load()) {
            bool need_notify = queue_.empty() || queue_.top().delay() > delay;
            queue_.push(DelayedExecutable(std::move(func), delay));
            lk.unlock();
            if (need_notify) {
                queue_cond_.notify_one();
            }
        }
    }

    void shutdown(bool wait_for_complete = true) {
        // 修改后立即生效，在 run_loop 当中就能尽早（加锁前）就检测到 is_active 的变化
        active_.store(false, std::memory_order_relaxed);
        if (!wait_for_complete) {
            std::unique_lock lock(mut_);
            // 清空任务队列
            decltype(queue_) empty_queue;
            std::swap(queue_, empty_queue);
            lock.unlock();
        }

        // 通知 wait 函数，避免 Looper 线程不退出
        // 不需要加锁，否则锁会交给 wait 方导致当前线程阻塞
        queue_cond_.notify_all();
    }

    void join() {
        if (worker_.joinable()) {
            worker_.join();
        }
    }
};

}  // namespace coro

#endif
