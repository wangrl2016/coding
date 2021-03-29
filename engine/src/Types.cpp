//
// Created by wangrl on 2021/3/29.
//

#include <cstdarg>
#include <cstdio>

void DebugF(const char format[], ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}