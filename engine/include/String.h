//
// Created by wangrl on 2021/3/21.
//

#pragma once

#include <cstdio>
#include <string>
#include <atomic>
#include <memory>

#include "RefCnt.h"

/**
 * Light weight class for managing strings. Uses reference
 * counting to make string assignments and copies very fast
 * with no extra RAM cost. Assume UTF8 encoding.
 */

class String {
public:
    // 默认构造器
    String();

    explicit String(size_t len);

    explicit String(const char text[]);

    String(const char text[], size_t len);

    // 复制构造器
    String(const String&);

    // 移动构造器
    String(String&&);

    explicit String(const std::string&);

    ~String();

private:
    struct Rec {
    public:

        constexpr Rec(uint32_t len, int32_t refCnt) : fLength(len), fRefCnt(refCnt) {}

        /**
         * std::shared_ptr介绍
         *
         * https://en.cppreference.com/w/cpp/memory/shared_ptr
         */
        static SharedPtr<Rec> Make(const char text[], size_t len);

        char* data() { return &fBeginningOfData; }

        const char* data() const { return &fBeginningOfData; }

        void ref() const;

        void unref() const;

        bool unique() const;

        // 字符串的长度
        uint32_t fLength;   // logically size_t, but we want it to stay 32 bits
        mutable std::atomic<int32_t> fRefCnt;
        char fBeginningOfData = '\0';

    private:
        // Ensure the unsized deleted is called
        void operator delete(void* p) { ::operator delete(p); }
    };

    SharedPtr<Rec> fRec;

    const String& validate() const {
        return *this;
    }

    static const Rec gEmptyRec;
};
