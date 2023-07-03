//
// Created by zhaojieyi on 2023/5/10.
//

#ifndef CODESNIPPET_CALLBACK_TIMER_H
#define CODESNIPPET_CALLBACK_TIMER_H

#include <atomic>
#include <functional>
#include "timer_base.h"

/**
 * callback_timer: short for CallBackTimer, for what need to be done is done by a callback func
 */
class CallbackTimer : public TimerBase {
 public:
    typedef std::function<void()> Callback;

    CallbackTimer(int interval, Callback func);
    virtual ~CallbackTimer() {}

    void Start();
    void Cancel();

    void Timeout() override;

 private:
    struct Impl {
        Impl(int interval, Callback func) : interval(interval), func(func) {}

        int interval;
        Callback func;
        std::atomic<bool> active{true};
    };

    std::shared_ptr<Impl> pimpl_;
};

#endif  // CODESNIPPET_CALLBACK_TIMER_H
