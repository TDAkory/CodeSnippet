//
// Created by zhaojieyi on 2023/6/2.
//

#include "scope_guard.h"

ScopeGuard::ScopeGuard(std::function<void()> on_exit): on_exit_func_(std::move(on_exit)),do_exit_(true) {}

ScopeGuard::~ScopeGuard() noexcept {
    if (do_exit_) {
        on_exit_func_();
    }
}
