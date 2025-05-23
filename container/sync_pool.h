//
// Created by zhaojieyi on 2023/5/22.
//

#ifndef CODESNIPPET_SYNC_POOL_H
#define CODESNIPPET_SYNC_POOL_H

#include <mutex>

namespace safe_container {

template <typename T>
class SyncPool {
    friend class TestSyncPool_TestNormalOps_Test;

 public:
    SyncPool();

    explicit SyncPool(std::size_t cap);

    SyncPool(std::size_t cap, std::size_t t_cap);

    ~SyncPool();

    SyncPool(const SyncPool &) = delete;

    SyncPool &operator=(const SyncPool &) = delete;

    SyncPool(SyncPool &&) = delete;

    SyncPool &operator=(SyncPool &&) = delete;

    T *Get();

    void Put(T *t);

    std::size_t Cap();

    std::size_t CurIdx();

 public:
    static constexpr std::size_t default_cap = 16;

 private:
    std::size_t cap_{0};  // stack capacity
    std::size_t idx_{0};  // stack current idx

    std::size_t t_cap_{0};  // T capacity for memory alloc
    T **stack_{nullptr};    // T pointers
    std::mutex mut_;
};

template <typename T>
SyncPool<T>::SyncPool() : cap_(default_cap) {
    stack_ = new T *[cap_];
}

template <typename T>
SyncPool<T>::SyncPool(std::size_t cap) : cap_(cap) {
    stack_ = new T *[cap_];
    for (std::size_t i = 0; i < cap_; i++) {
        stack_[i] = new T();
    }
    idx_ = cap_;
}

template <typename T>
SyncPool<T>::SyncPool(std::size_t cap, std::size_t t_cap) : cap_(cap), t_cap_(t_cap) {
    stack_ = new T *[cap_];
    for (std::size_t i = 0; i < cap_; i++) {
        stack_[i] = new T();
    }
    idx_ = cap_;
}

template <typename T>
SyncPool<T>::~SyncPool() {
    if (nullptr != stack_) {
        for (std::size_t i = 0; i < cap_; i++) {
            if (stack_[i]) {
                delete stack_[i];
            }
        }
        delete stack_;
        stack_ = nullptr;
    }
}

template <typename T>
inline T *SyncPool<T>::Get() {
    {
        std::lock_guard<std::mutex> lk(mut_);
        if (idx_ > 0) {
            return stack_[--idx_];
        }
    }
    return new T;
}

template <typename T>
inline void SyncPool<T>::Put(T *t) {
    if (nullptr == t) {
        return;
    }
    {
        std::lock_guard<std::mutex> lk(mut_);
        if (idx_ < cap_) {
            stack_[idx_++] = t;
            return;
        }
    }
    delete t;
}

template <typename T>
inline std::size_t SyncPool<T>::Cap() {
    return cap_;
}

template <typename T>
inline std::size_t SyncPool<T>::CurIdx() {
    std::lock_guard<std::mutex> lk(mut_);
    return idx_;
}
}  // namespace safe_container

#endif  // CODESNIPPET_SYNC_POOL_H
