//
// Created by wangrl on 2021/3/22.
//

#pragma once

template<typename T>
constexpr inline bool IsPowOfTwo(T value) {
    return (value & (value - 1)) == 0;
}

static inline bool FloatIsNan(float x) {
    return x != x;
}
