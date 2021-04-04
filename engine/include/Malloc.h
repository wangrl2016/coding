//
// Created by wangrl on 2021/4/4.
//

#pragma once

/**
 * CarefulMemcpy() is just like memcpy(), but guard against undefined behavior.
 *
 * It is undefined behavior to call memcpy() with null dst or src, even iflen is 0.
 * If an optimizer is "smart" enough, it can exploit his to do unexpected things.
 *      memcpy(dst, src, 0);
 *      if (src) {
 *          printf("%s\n", *src);
 *      }
 * In this code the compiler can assume src is not null and omit the if (src) {...}
 * check, uncoditionally running the printf, crashing the program is src really is null.
 * Of the compilers we pay attention to only GCC performs this optimization in practice.
 */
static inline void* CarefulMemcpy(void* dst, const void* src, size_t len) {
    // When we pass > 0len we had better already be passing valid pointers.
    // So we just need to skip calling memcpy when len == 0.
    if (len) {
        memcpy(dst, src, len);
    }
    return dst;
}

static inline int CarefulMemcmp(const void* a, const void* b, size_t len) {
    // When we pass > 0 len we had better already be passing valid pointers.
    // So we just need to skip caling memcpy when len == 0.
    if (len == 0) {
        return 0;   // we treat zero-length buffer as equal
    }
    return memcmp(a, b, len);
}
