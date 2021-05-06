//
// Created by wangrl2016 on 2021/5/6.
//

#pragma once

#if (defined(__linux) || defined(__unix) || defined(__posix) || defined(__LINUX__) || defined(__linux__))
#define PLATFORM_LINUX 1
#elif (defined(_WIN64) || defined(_WIN32) || defined(__CYGWIN32__) || defined(__MINGW32__))
#define PLATFORM_WINDOWS 1
#elif (defined(MACOSX) || defined(__DARWIN__) || defined(__APPLE__))
#define PLATFORM_OSX 1
#endif

#if (defined(WIN_32) || defined(__i386__) || defined(i386) || defined(__x86__))
#define ARCH_32 1
#elif (defined(__amd64) || defined(__amd64__) || defined(__x86_64) || defined(__x86_64__) || defined(_M_X64) || defined(__ia64__) || defined(_M_IA64))
#define ARCH_64 1
#endif

#if (defined(__clang__))
#define COMPILER_CLANG 1
#elif (defined(__GNUC__))
#define COMPILER_GCC 1
#elif (defined _MSC_VER)
#define COMPILER_VISUAL_STUDIO 1
#endif

#if ((_M_IX86_FP) && (_M_IX86_FP >= 2)) || (_M_AMD64) || defined(_M_X64)
#define __SSE2__
#endif

#if defined(__ARM_NEON__)
#define ARM_NEON_INTRINSICS 1
#endif

#if defined(LABSOUND_PLATFORM_OSX) || defined(LABSOUND_PLATFORM_LINUX)
#include <cmath>
#endif




