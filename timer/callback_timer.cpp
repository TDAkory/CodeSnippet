//
// Created by zhaojieyi on 2023/5/10.
//

#include "callback_timer.h"
#include <chrono>
#include <iostream>
#include <thread>

CallbackTimer::CallbackTimer(int interval, Callback func) {
    pimpl_ = std::make_shared<Impl>(interval, func);
}

void CallbackTimer::Start() {
    std::thread t([=]() {
        if (!pimpl_->active.load()) {
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(pimpl_->interval));
        if (!pimpl_->active.load()) {
            return;
        }
        pimpl_->func();
    });
    t.detach();
}

void CallbackTimer::Cancel() {
    pimpl_->active.store(false);
}

void CallbackTimer::Timeout() {}