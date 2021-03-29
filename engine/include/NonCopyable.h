//
// Created by wangrl on 2021/3/29.
//

#pragma once

#include <cstdio>
#include "Types.h"


class NonCopyable {
public:
    NonCopyable() {
        PRINT_FUNC();
    }

    NonCopyable(NonCopyable&&) noexcept {
        PRINT_FUNC();
    };

    NonCopyable& operator=(NonCopyable&&) noexcept {
        PRINT_FUNC();
        return *this;
    };

    ~NonCopyable() {
        PRINT_FUNC();
    }

    NonCopyable(const NonCopyable&) = delete;

    NonCopyable& operator=(const NonCopyable&) = delete;
};
