//
// Created by zhaojieyi on 2023/7/3.
//

#ifndef CODESNIPPET_FUNC_DECERATOR_H
#define CODESNIPPET_FUNC_DECERATOR_H

#include <iostream>

template <typename F, class PRE, class POST = void (*)(...)>
struct Decorator {
    Decorator(F &func, PRE &&pre, POST &&post = POST()) {
        func = [func = std::move(func), pre = std::move(pre), post = std::move(post)](auto &&...args) {
            if (pre) {
                auto result = pre(args...);
                if (result) {
                    return std::move(*result);
                }
            }
            auto result = func(std::forward<decltype(args)>(args)...);
            if (post) {
                post(result, args...);
            }
            return result;
        };
    }
};

#define DECORATOR(f, decorators...) auto Decorator##__FILE__##__LINE__ = Decorator(f, ##decorators);

#endif  // CODESNIPPET_FUNC_DECERATOR_H
