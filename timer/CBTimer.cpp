//
// Created by zhaojieyi on 2023/5/10.
//

#include "CBTimer.h"
#include <chrono>
#include <thread>

CBTimer::CBTimer(int interval, Callback func) {
  pimpl_ = std::make_shared<Impl>(interval, func);
}

void CBTimer::Start() {
  std::thread t([pimpl=pimpl_](){
    if (!pimpl->active.load()) {
      return;
    }
    std::this_thread::sleep_for(std::chrono::seconds(pimpl->interval));
    if (!pimpl->active.load()) {
      return;
    }
    pimpl->func();
  });
  t.detach();
}

void CBTimer::Cancel() {
  pimpl_->active.store(false);
}

void CBTimer::Timeout() {}