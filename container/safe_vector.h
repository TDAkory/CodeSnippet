//
// Created by zhaojieyi on 2022/5/15.
//

#ifndef CONTAINER_SAFE_VECTOR_H
#define CONTAINER_SAFE_VECTOR_H

#include <mutex>
#include <vector>

namespace safe_container {

template <typename T>
class LVector {
 public:
    LVector() = default;

    ~LVector() = default;

    LVector(const LVector<T> &v) { v_ = v.v_; }

    LVector(LVector<T> &&v) noexcept {
        v_ = v.v_;
        v.v_.clear();
    }

    void push_back(const T &value) noexcept {
        m_.lock();
        v_.push_back(value);
        m_.unlock();
    }

    T operator[](unsigned int idx) { return v_[idx]; }

 private:
    std::mutex m_;
    std::vector<T> v_;
};

template <typename T>
class LFVector {
 public:
    LFVector() = default;

    ~LFVector() = default;

    LFVector(const LFVector &v) {
        v_ = v.v_;
        flag_.store(false);
    }

    LFVector(LFVector &&v) noexcept {
        v_ = v.v_;
        flag_.store(false);
    }

    void replace(const int idx, const T &value) noexcept {
        lock();
        v_[idx] = value;
        unlock();
    }

    void push_back(const T &value) noexcept {
        lock();
        v_.push_back(value);
        unlock();
    }

    T operator[](unsigned int idx) { return v_[idx]; }

 private:
    void lock() {
        bool expect = false;
        while (!flag_.compare_exchange_weak(expect, true)) {
            expect = false;
        }
    }

    void unlock() { flag_.store(false); }

 private:
    std::atomic<bool> flag_{false};
    std::vector<T> v_;
};

template <typename T>
class TLVector {
 public:
    thread_local static std::vector<T> vec_;

    TLVector() = default;

    ~TLVector() = default;

    TLVector(const TLVector &v) {
        v_ = v.v_;
        vec_ = v.vec_;
    }

    TLVector(TLVector &&v) noexcept {
        v_ = v.v_;
        vec_ = v.vec_;
    }

    void push_back(const T &value) { vec_.push_back(value); }

    void merge() { v_.insert(v_.end(), vec_.begin(), vec_.end()); }

 private:
    std::vector<T> v_;
    std::mutex m_;
};

template <typename T>
thread_local std::vector<T> TLVector<T>::vec_;
}  // namespace safe_container

#endif  // CONTAINER_SAFE_VECTOR_H
