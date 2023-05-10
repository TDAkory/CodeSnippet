//
// Created by zhaojieyi on 2023/5/8.
//

#ifndef CODESNIPPET_TIMERBASE_H
#define CODESNIPPET_TIMERBASE_H

class TimerBase {
public:
  virtual ~TimerBase() {}

  virtual void Timeout() = 0;
};

#endif // CODESNIPPET_TIMERBASE_H
