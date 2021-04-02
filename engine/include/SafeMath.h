//
// Created by wangrl on 2021/4/2.
//

#pragma once

template<typename T, class Enable = void>
struct StripEnum {
    typedef T type;
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
 * The two remaining check s -> U [v >= 0] and U -> s [v <= max(s)] can be donewith one op.
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
            src <= (S) std::numeric_limits<typename
}
