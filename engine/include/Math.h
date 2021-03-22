//
// Created by wangrl on 2021/3/22.
//

#pragma once

#include "gtest/gtest.h"

template<typename T>
constexpr inline bool IsPowOfTwo(T value) {
    return (value & (value - 1)) == 0;
}

TEST(Math, IsPowOfTwo) {
    ASSERT_TRUE(IsPowOfTwo(16));
    ASSERT_FALSE(IsPowOfTwo(15));
}
