//
// Created by zhaojieyi on 2023/5/10.
//

#include "CBTimer.h"
#include <cstdlib>
#include <string>
#include <memory>
#include <iostream>
#include <tuple>

struct Alarm {
  Alarm(int seconds_, const std::string& message_):seconds(seconds_), message(message_) {
  }

  int seconds;
  std::string message;
};

void callback(std::shared_ptr<Alarm> alarm) {
  std::cout << "(" << alarm->seconds << ") " << alarm->message << std::endl;
}

int main()
{
      auto alarm = std::make_shared<Alarm>(1, "hello world");
      CBTimer t(1, std::bind(callback, alarm));
      t.Start();
}
