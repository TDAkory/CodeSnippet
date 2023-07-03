//
// Created by zhaojieyi on 2023/7/3.
//

#ifndef CODESNIPPET_TIMER_DECORATOR_H
#define CODESNIPPET_TIMER_DECORATOR_H

#include <chrono>
#include <iostream>

template <typename T>
auto TimeIt(T &&func) {
    return [=](auto &&...args) {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = func(std::forward<decltype(args)>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "Time taken by function " << duration << " microseconds." << std::endl;
        return result;
    };
}

#endif  // CODESNIPPET_TIMER_DECORATOR_H
