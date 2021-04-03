//
// Created by wangrl on 2021/3/22.
//

#include "gtest/gtest.h"
#include "include/SafeMath.h"
#include "include/String.h"

TEST(String, SafeMath) {    // NOLINT
    size_t max = std::numeric_limits<size_t>::max();
    {
        size_t halfMax = max >> 1;
        size_t halfMaxPlusOne = halfMax + 1;
        SafeMath safe;
        ASSERT_TRUE(safe.add(halfMax, halfMax) == 2 * halfMax);
        ASSERT_TRUE(safe);
        ASSERT_TRUE(safe.add(halfMax, halfMaxPlusOne) == max);
        ASSERT_TRUE(safe);
        ASSERT_TRUE(safe.add(max, 1) == 0);
        ASSERT_FALSE(safe);
    }
    {
        SafeMath safe;
        (void) safe.add(max, max);
        ASSERT_TRUE(!safe);
    }
    {
        size_t bits = (sizeof(size_t) * 8);
        size_t halfBits = bits / 2;
        size_t sqrtMax = max >> halfBits;
        size_t sqrtMaxPlusOne = sqrtMax + 1;
        SafeMath safe;
        ASSERT_TRUE(safe.mul(sqrtMax, sqrtMax) == sqrtMax * sqrtMax);
        ASSERT_TRUE(safe);
        ASSERT_TRUE(safe.mul(sqrtMax, sqrtMaxPlusOne) == sqrtMax << halfBits);
        ASSERT_TRUE(safe);
        ASSERT_TRUE(safe.mul(sqrtMaxPlusOne, sqrtMaxPlusOne) == 0);
        ASSERT_TRUE(!safe);
    }
    {
        SafeMath safe;
        (void) safe.mul(max, max);
        ASSERT_TRUE(!safe);
    }
}

TEST(String, String) {  // NOLINT
    String a;
    String b((size_t) 0);
}