//
// Created by wangrl on 2021/3/22.
//

#include "gtest/gtest.h"
#include "include/SafeMath.h"
#include "include/String.h"
#include "include/Sort.h"

TEST(String, SafeMath) {    // NOLINT
    size_t max = std::numeric_limits<size_t>::max();
    {
        size_t halfMax = max >> 1u;
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
    String c("");
    String d(nullptr, 0);

    ASSERT_TRUE(a.isEmpty());
    ASSERT_TRUE(a == b && a == c && a == d);

    a.set("hello");
    b.set("helloX", 5);
    c.set(a);
    d.resize(5);
    memcpy(d.writable_str(), "helloZ", 5);

    ASSERT_TRUE(!a.isEmpty());
    ASSERT_TRUE(a.size() == 5);

    ASSERT_TRUE(a == b && a == c && a == d);
    ASSERT_TRUE(a.equals("hello", 5));
    ASSERT_TRUE(a.equals("hello"));
    ASSERT_TRUE(!a.equals("help"));

    ASSERT_TRUE(a.startsWith("hell"));
    ASSERT_TRUE(a.startsWith('h'));
    ASSERT_TRUE(!a.startsWith("ell"));
    ASSERT_TRUE(!a.startsWith('e'));
    ASSERT_TRUE(a.startsWith(""));
    ASSERT_TRUE(a.endsWith("llo"));
    ASSERT_TRUE(a.endsWith('o'));
    ASSERT_TRUE(!a.endsWith("ll"));
    ASSERT_TRUE(!a.endsWith('l'));
    ASSERT_TRUE(a.endsWith(""));
    ASSERT_TRUE(a.contains("he"));
    ASSERT_TRUE(a.contains("ll"));
    ASSERT_TRUE(a.contains("lo"));
    ASSERT_TRUE(a.contains("hello"));
    ASSERT_TRUE(!a.contains("helloHello"));
    ASSERT_TRUE(a.contains(""));
    ASSERT_TRUE(a.contains('e'));
    ASSERT_TRUE(!a.contains('z'));

    String e(a);
    String f("hello");
    String g("helloZ", 5);
    ASSERT_TRUE(a == e && a == f && a == g);

    b.set("world");
    c = b;
    ASSERT_TRUE(a != b && a != c && b == c);

    a.append(" world");
    e.append("worldZ", 5);
    e.insert(5, " ");
    f.set("world");
    f.prepend("hello ");
    ASSERT_TRUE(a.equals("hello world") && a == e && a == f);

    a.reset();
    b.resize(0);
    ASSERT_TRUE(a.isEmpty() && b.isEmpty() && a == b);

    a.set("a");
    a.set("ab");
    a.set("abc");
    a.set("abcd");

    a.set("");
    a.appendS32(0x7FFFFFFFL);
    ASSERT_TRUE(a.equals("2147483647"));

    a.set("");
    a.appendS32(0x80000001L);
    ASSERT_TRUE(a.equals("-2147483647"));

    a.set("");
    a.appendS32(0x80000000L);
    ASSERT_TRUE(a.equals("-2147483648"));

    a.set("");
    a.appendU32(0x7FFFFFFFUL);
    ASSERT_TRUE(a.equals("2147483647"));
    a.set("");
    a.appendU32(0x80000001UL);
    ASSERT_TRUE(a.equals("2147483649"));
    a.set("");
    a.appendU32(0xFFFFFFFFUL);
    ASSERT_TRUE(a.equals("4294967295"));

    a.set("");
    a.appendS64(0x7FFFFFFFFFFFFFFFLL, 0);
    ASSERT_TRUE(a.equals("9223372036854775807"));
    a.set("");
    a.appendS64(0x8000000000000001LL, 0);
    ASSERT_TRUE(a.equals("-9223372036854775807"));
    a.set("");
    a.appendS64(0x8000000000000000LL, 0);
    ASSERT_TRUE(a.equals("-9223372036854775808"));
    a.set("");
    a.appendS64(0x0000000001000000LL, 15);
    ASSERT_TRUE(a.equals("000000016777216"));
    a.set("");
    a.appendS64(0xFFFFFFFFFF000000LL, 15);
    ASSERT_TRUE(a.equals("-000000016777216"));

    a.set("");
    a.appendU64(0x7FFFFFFFFFFFFFFFULL, 0);
    ASSERT_TRUE(a.equals("9223372036854775807"));
    a.set("");
    a.appendU64(0x8000000000000001ULL, 0);
    ASSERT_TRUE(a.equals("9223372036854775809"));
    a.set("");
    a.appendU64(0xFFFFFFFFFFFFFFFFULL, 0);
    ASSERT_TRUE(a.equals("18446744073709551615"));
    a.set("");
    a.appendU64(0x0000000001000000ULL, 15);
    ASSERT_TRUE(a.equals("000000016777216"));

    static const struct {
        float fValue;
        const char* fString;
    } gRec[] = {
            {0, "0"},
    };

    for (size_t i = 0; i < ARRAY_SIZE(gRec); i++) {
        a.reset();
        a.appendFloat(gRec[i].fValue);
    }
}
