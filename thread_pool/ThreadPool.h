//
// Created by zhaojieyi on 2022/9/16.
//

#ifndef CODESNIPPET_THREADPOOL_H
#define CODESNIPPET_THREADPOOL_H

#pragma once
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>
#include "SafeQueue.h"

class ThreadPool {
public:
    ThreadPool(const int n_threads) : shutdown_(false), threads_(std::vector<std::thread>(n_threads)) {}

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;

    void init() {
        for(std::size_t i = 0; i < threads_.size(); i++) {
            threads_[i] = std::thread(ThreadWorker(this, i));
        }
    }

    void shutdown() {
        shutdown_ = true;
        conditional_lock_.notify_all();
        std::cout << "Task Left: " << queue_.size() << std::endl;
        for(std::size_t i = 0; i < threads_.size(); i++) {
            if (threads_[i].joinable()) {
                threads_[i].join();
            }
        }
    }

    template<typename F, typename ...Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

        std::function<void()> wrapper_func = [task_ptr]() {
            (*task_ptr)();
        };

        queue_.enqueue(wrapper_func);
        conditional_lock_.notify_one();

        return task_ptr->get_future();
    }


private:
    class ThreadWorker {
    public:
        ThreadWorker(ThreadPool *p, const std::size_t id) : pool_(p), id_(id) {}

        void operator()() {
            std::function<void()> func;
            bool dequeued = false;
            while (!pool_->shutdown_) {
                std::unique_lock<std::mutex> lock(pool_->conditional_mutex_);
                if (pool_->queue_.empty()) {
                    pool_->conditional_lock_.wait(lock);
                }
                dequeued = pool_->queue_.dequeue(func);

                if (dequeued) {
                    func();
                    dequeued = false;
                }
            }
        }
    private:
        ThreadPool *pool_;
        std::size_t id_;
    };

    bool shutdown_;
    SafeQueue<std::function<void()>> queue_;
    std::vector<std::thread> threads_;
    std::mutex conditional_mutex_;
    std::condition_variable conditional_lock_;
};

#endif //CODESNIPPET_THREADPOOL_H
