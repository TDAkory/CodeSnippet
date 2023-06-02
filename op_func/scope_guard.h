//
// Created by zhaojieyi on 2023/6/2.
//

#ifndef CODESNIPPET_SCOPE_GUARD_H
#define CODESNIPPET_SCOPE_GUARD_H

#include <functional>

class ScopeGuard final{
public:
    ScopeGuard() = default;
    explicit ScopeGuard(std::function<void()> on_exit);
    ~ScopeGuard() noexcept;

private:
    std::function<void()> on_exit_func_;
    bool do_exit_{true};
};

#endif //CODESNIPPET_SCOPE_GUARD_H
