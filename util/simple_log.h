//
// Created by zhaojieyi on 2024/5/17.
//

#ifndef CODESNIPPET_SIMPLE_LOG_H
#define CODESNIPPET_SIMPLE_LOG_H

#include <iostream>

const char *file_name(const char *path);

void PrintTime();

void PrintThread();

template <typename... U>
void Println(U... u) {
    using namespace std;

    int i = 0;
    auto printer = [&i]<typename Arg>(Arg arg) {
        if (sizeof...(U) == ++i)
            cout << arg << endl;
        else
            cout << arg << " ";
    };
    (printer(u), ...);

    std::cout.flush();
}

#define DEBUG(...)                                                   \
    PrintTime();                                                     \
    PrintThread();                                                   \
    printf("(%s:%d) %s: ", file_name(__FILE__), __LINE__, __func__); \
    Println(__VA_ARGS__);
#endif  // CODESNIPPET_SIMPLE_LOG_H
