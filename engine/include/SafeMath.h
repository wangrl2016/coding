//
// Created by wangrl on 2021/4/2.
//

#pragma once

#include <limits>
#include <cstdint>
#include <type_traits>

template<typename T, class Enable = void>
struct StripEnum {
    typedef T type;
};

template<typename T>
struct StripEnum<T, typename std::enable_if<std::is_enum<T>::value>::type> {
    typedef typename std::underlying_type<T>::type type;
};


/**
 * In C++ an unsigned to signed cast where the source value cannot be represented in the destination
 * type results in an implementation defined destination value. Unlike C, C++ does not allow a trap.
 * This make "(S)(D)s == s" a possibly useful test. However, there are two cases where thia is
 * incorrect:
 *
 * 小的带符号整形不能表示大的不带符号整形
 * when testing if a value of a smaller signed type can be represented in a larger unsigned type
 * (int8_t)(uint16_t)-1 == -1 => (int8_t)0xFFFF == -1 => [implementation defined] == -1
 *
 * 大的不带符号整形可以表示小的带符号整形
 * when testing if a value of a larger unsigned type can be represented in a smaller signed type
 * (uint16_t)(int8_t)0xFFFF == 0xFFFF => (uint16_t)-1 == 0xFFFF => 0xFFFF == 0xFFFF => true.
 *
 * Consider the cases:
 *  u = unsigned, less digits
 *  U = unsigned, more digits
 *  s = signed, less digits
 *  S = signed, more digits
 *  v is the value we're considering.
 *
 *  u -> U: (u)(U)v == v, trivially true     // 将v变成大的无符号整形然后变回来显而易见是正确的
 *　U -> u: (U)(u)v == v, both casts well defined, test works
 *  s -> S: (s)(S)v == v, trivially true
 *  S -> s: (S)(s)v == v, first cast implementation value, second cast defined, test works
 *  s -> U: (s)(U)v == v, *this is bad*, the second cast result in implementation defined value
 *  S -> u: (S)(u)v == v, the second cast is required to prevent promotion of rhs to unsgined
 *  u -> S: (u)(S)v == v, trivially true
 *  U -> s: (U)(s)v == v, *this is bad*
 *                  first cast results in implementation defined value,
 *                  second cast is defined. However, this creates false positives
 *                  uint16_tx = 0xFFFF
 *                  (uint16_t)(int8_t)x == x
 *              =>  (uint16_t)-1        == x
 *              =>  0xFFFF              == x
 *              =>  true
 *
 * So for the eight cases three are trivially true, three more are valid casts, and two are special.
 * The two 'full' checks which otherwise require two comparisons are valid cast checks.
 * The two remaining check s -> U [v >= 0] and U -> s [v <= max(s)] can be done with one op.
 */

template<typename D, typename S>
static constexpr inline
typename std::enable_if<(std::is_integral<S>::value || std::is_enum<S>::value) &&
                        (std::is_integral<D>::value || std::is_enum<D>::value), bool>::type
TFitsIn(S src) {    // return bool
    // TFitsIn() is used in public headers, so needs to be written targeting at most C++11.
    return
        // E.g. (int8_t)(uint8_t) int8_t(-1) == -1, but the uint8_t == 255, not -1.
            (std::is_signed<S>::value && std::is_unsigned<D>::value && sizeof(S) <= sizeof(D)) ?
            (S) 0 <= src :
            // E.g. (uint8_t)(int8_t) uint8_t(255) == 255, but the int8_t == -1.
            (std::is_signed<D>::value && std::is_unsigned<S>::value && sizeof(D) <= sizeof(S)) ?
            src <= (S) std::numeric_limits<typename StripEnum<D>::type>::max() :
            // More complex version that's safe with /RTCc. Used in all debug builds, for coverage.
            (std::is_signed<S>::value) ?
            (intmax_t) src >= (intmax_t) std::numeric_limits<typename StripEnum<D>::type>::min() &&
            (intmax_t) src <= (intmax_t) std::numeric_limits<typename StripEnum<D>::type>::max() :
            // std::is_unsigned<S> ?
            (uintmax_t) src <= (uintmax_t) std::numeric_limits<typename StripEnum<D>::type>::max();
}

/**
 * SafeMath always check that a series of operations do not overflow.
 * This must be correct for all platforms, because this is a check for safety at runtime.
 */
class SafeMath {
    SafeMath() = default;

    bool ok() const {
        return fOK;
    }

    explicit operator bool() const {
        return fOK;
    }

    size_t mul(size_t x, size_t y) {
        return sizeof(size_t) == sizeof(uint64_t) ? mul64(x, y) : mul32(x, y);
    }

    size_t add(size_t x, size_t y) {
        size_t result = x + y;
        fOK &= result >= x;
        return result;
    }

    /**
     * Return a + b, unless this result is an overflow/underflow. In those cases, fOK
     * will be set to false, and it is undefined what this returns.
     */
    int addInt(int a, int b) {
        if (b < 0 && a < std::numeric_limits<int>::min() - b) {
            fOK = false;
            return a;
        } else if (b > 0 && a > std::numeric_limits<int>::max() - b) {
            fOK = false;
            return a;
        }
        return a + b;
    }

    // 2的阶乘取整
    size_t alignUp(size_t x, size_t alignment) {
        assert(alignment && !(alignment & (alignment - 1)));
        return add(x, alignment - 1) & ~(alignment - 1);
    }

    template<typename T>
    T castTo(size_t value) {
        if (!TFitsIn<T>(value)) {
            fOK = false;
        }
        return static_cast<T>(value);
    }

    // These saturate to their results.
    static size_t Add(size_t x, size_t y);

    static size_t Mul(size_t x, size_t y);

    static size_t Align4(size_t x) {
        SafeMath safe;
        return safe.alignUp(x, 4);
    }

private:
    uint32_t mul32(uint32_t x, uint32_t y) {
        uint64_t bx = x;
        uint64_t by = y;
        uint64_t result = bx * by;
        fOK &= result >> 32 == 0;   // 说明没有溢出
        return result;
    }

    uint64_t mul64(uint64_t x, uint64_t y) {
        if (x <= std::numeric_limits<uint64_t>::max() >> 32 &&
            y <= std::numeric_limits<uint64_t>::max() >> 32) {
            return x * y;
        } else {
            auto hi = [](uint64_t x) { return x >> 32; };
            auto lo = [](uint64_t x) { return x & 0xFFFFFFFF; };

            uint64_t lx_ly = lo(x) * lo(y);
            uint64_t hx_ly = hi(x) * lo(y);
            uint64_t lx_hy = lo(x) * hi(y);
            uint64_t hx_hy = hi(x) * hi(y);
            uint64_t result = 0;
            result = this->add(lx_ly, (hx_ly << 32));
            result = this->add(result, (lx_hy << 32));
            fOK &= (hx_hy + (hx_ly >> 32) + (lx_hy >> 32)) == 0;
            return result;
        }
    }

    bool fOK = true;
};