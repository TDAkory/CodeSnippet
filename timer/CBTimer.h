//
// Created by zhaojieyi on 2023/5/10.
//

#ifndef CODESNIPPET_CBTIMER_H
#define CODESNIPPET_CBTIMER_H

#include <functional>
#include <atomic>
#include "TimerBase.h"

/**
 * CBTimer: short for CallBackTimer, for what need to be done is done by a callback func
 */
class CBTimer : public TimerBase {
public:
  typedef std::function<void()> Callback;

  CBTimer(int interval, Callback func);
  virtual ~CBTimer() {}

  void Start();
  void Cancel();

  void Timeout() override;

      private:
  struct Impl {
    Impl(int interval, Callback func):interval(interval), func(func) {}

    int interval;
    Callback  func;
    std::atomic<bool> active{true};
  };

  std::shared_ptr<Impl> pimpl_;
};

#endif // CODESNIPPET_CBTIMER_H
