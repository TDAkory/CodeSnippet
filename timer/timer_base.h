//
// Created by zhaojieyi on 2023/5/8.
//

#ifndef CODESNIPPET_TIMER_BASE_H
#define CODESNIPPET_TIMER_BASE_H

class TimerBase {
 public:
    virtual ~TimerBase() {}

    virtual void Timeout() = 0;
};

#endif  // CODESNIPPET_TIMER_BASE_H
