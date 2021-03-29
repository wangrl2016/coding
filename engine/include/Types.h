//
// Created by wangrl on 2021/3/29.
//

#pragma once

#if !defined(DEBUG) && !defined(RELEASE)
#ifdef NDEBUG
#define RELEASE
#else
#define DEBUG
#endif
#endif

#if defined(DEBUG) && defined(RELEASE)
#error "Cannot define both DEBUG and RELEASE"
#elif !defined(DEBUG) && !defined(RELEASE)
#error "must define either DEBUG or RELEASE"
#endif

void DebugF(const char format[], ...);

#ifdef DEBUG
#define DEBUG_F(...)                DebugF(__VA_ARGS__)
#define PRINT_FUNC()                DebugF("%s line %d\n", __func__, __LINE__)
#else

#endif