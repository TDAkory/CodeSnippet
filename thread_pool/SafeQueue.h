//
// Created by zhaojieyi on 2022/9/16.
//

#ifndef CODESNIPPET_SAFEQUEUE_H
#define CODESNIPPET_SAFEQUEUE_H

#pragma once
#include <mutex>
#include <queue>

template<typename T>
class SafeQueue {
public:
    SafeQueue(){}
    ~SafeQueue(){}

    bool empty() {
        std::unique_lock<std::mutex> lk(m_mutex);
        return m_queue.empty();
    }

    std::size_t size() {
        std::unique_lock<std::mutex> lk(m_mutex);
        return m_queue.size();
    }

    void enqueue(T &t) {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_queue.push(t);
    }

    bool dequeue(T &t) {
        std::unique_lock<std::mutex> lk(m_mutex);
        if (m_queue.empty()) {
            return false;
        }
        t = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
};

#endif //CODESNIPPET_SAFEQUEUE_H

