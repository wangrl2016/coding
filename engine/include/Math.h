//
// Created by wangrl on 2021/3/22.
//

#pragma once

constexpr int32_t gFloatBitsExponentMask = 0x7F800000;
constexpr int32_t gFloatBitsMatissaMask = 0x007FFFFF;

static inline bool FloatBitsIsFinite(int32_t bits) {
    return (bits & gFloatBitsExponentMask) != gFloatBitsExponentMask;
}

static inline bool FloatBitsInInf(int32_t bits) {
    return ((bits & gFloatBitsExponentMask) == gFloatBitsExponentMask) &&
           (bits & gFloatBitsMatissaMask) == 0;
}

union FloatIntUnion {
    float fFloat;
    int32_t fSignBitInt;
};

static inline int32_t Float2Bits(float x) {
    FloatIntUnion data;
    data.fFloat = x;
    return data.fSignBitInt;
}

template<typename T>
constexpr inline bool IsPowOfTwo(T value) {
    return (value & (value - 1)) == 0;
}

static inline bool FloatIsNaN(float x) {
    return x != x;
}

static inline bool FloatIsFinite(float x) {
    return FloatBitsIsFinite(Float2Bits(x));
}
