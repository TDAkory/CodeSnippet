//
// Created by zhaojieyi on 2023/5/10.
//

#include <iostream>
#include <memory>
#include <string>
#include "callback_timer.h"

struct Alarm {
    Alarm(int seconds_, const std::string &message_) : seconds(seconds_), message(message_) {}

    int seconds;
    std::string message;
};

void callback(std::shared_ptr<Alarm> alarm) {
    std::cout << "(" << alarm->seconds << ") " << alarm->message << std::endl;
}

int main() {
    auto alarm = std::make_shared<Alarm>(1, "hello world");
    callback_timer t(1, [alarm] { return callback(alarm); });
    t.Start();
}
