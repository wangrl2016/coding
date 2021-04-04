//
// Created by wangrl on 2021/3/21.
//

#pragma once

#include <cstdio>
#include <string>
#include <atomic>
#include <memory>
#include <cstring>

#include "RefCnt.h"
#include "SafeMath.h"

/**
 * Light weight class for managing strings. Uses reference
 * counting to make string assignments and copies very fast
 * with no extra RAM cost. Assume UTF8 encoding.
 */

static inline bool StrStartWith(const char string[], const char prefixStr[]) {
    assert(string);
    assert(prefixStr);
    return !strncmp(string, prefixStr, strlen(prefixStr));
}

static inline bool StrStartsWith(const char string[], const char prefixChar) {
    assert(string);
    return (prefixChar == *string);
}

bool StrEndsWith(const char string[], const char suffixStr[]);

bool StrEndsWith(const char string[], const char suffixChar);

int StrStartsWithOneOf(const char string[], const char prefixes[]);

static inline int StrFind(const char string[], const char subString[]) {
    const char* first = strstr(string, subString);
    if (nullptr == first) return -1;
    return ToInt(first - &string[0]);
}

static inline int StrFindLastOf(const char string[], const char subChar) {
    const char* last = strrchr(string, subChar);
    if (nullptr == last) return -1;
    return ToInt(last - &string[0]);
}

static inline bool StrContains(const char string[], const char subString[]) {
    assert(string);
    assert(subString);
    return (-1 != StrFind(string, subString));
}

static inline bool StrContains(const char string[], const char subChar) {
    assert(string);
    char tmp[2];
    tmp[0] = subChar;
    tmp[1] = '\0';
    return (-1 != StrFind(string, tmp));
}


class String {
public:
    // 默认构造器
    String();

    explicit String(size_t len);

    explicit String(const char text[]);

    String(const char text[], size_t len);

    String(const String&);

    String(String&&);

    explicit String(const std::string&);

    ~String();

    bool isEmpty() const {
        return 0 == fRec->fLength;
    }

    size_t size() const {
        return (size_t) fRec->fLength;
    }

    const char* c_str() const {
        return fRec->data();
    }

    char operator[](size_t n) const {
        return this->c_str()[n];
    }

    bool equals(const String&) const;

    bool equals(const char text[]) const;

    bool equals(const char text[], size_t len) const;

    bool startsWith(const char prefixStr[]) const {
        return StrStartWith(fRec->data(), prefixStr);
    }

    bool startsWith(const char prefixChar) const {
        return StrStartsWith(fRec->data(), prefixChar);
    }

    bool endsWith(const char suffixStr[]) const {
        return StrEndsWith(fRec->data(), suffixStr);
    }

    bool endsWith(const char suffixChar) const {
        return StrEndsWith(fRec->data(), suffixChar);
    }

    bool contains(const char subString[]) const {
        return StrContains(fRec->data(), subString);
    }

    bool contains(const char subChar) const {
        return StrContains(fRec->data(), subChar);
    }

    int find(const char subString[]) const {
        return StrFind(fRec->data(), subString);
    }

    int findLastOf(const char subChar) const {
        return StrFindLastOf(fRec->data(), subChar);
    }

    friend bool operator==(const String& a, const String& b) {
        return a.equals(b);
    }

    friend bool operator!=(const String& a, const String& b) {
        return !a.equals(b);
    }

    String& operator=(const String&);

    String& operator=(String&&);

    String& operator=(const char text[]);

    char* writable_str();

    char& operator[](size_t n) {
        return this->writable_str()[n];
    }

    void reset();

    void resize(size_t len);

    void set(const String& src) { *this = src; }

    void set(const char text[]);

    void set(const char text[], size_t len);

    void insert(size_t offset, const String& src) {
        this->insert(offset, src.c_str(), src.size());
    }

    void insert(size_t offset, const char text[]);

    void insert(size_t offset, const char text[], size_t len);

    void insertS32(size_t offset, int32_t value);

    void insertS64(size_t offset, int64_t value, int minDigits = 0);

    void append(const String& str) {
        this->insert((size_t) -1, str);
    }

    void append(const char text[]) {
        this->insert((size_t) -1, text);
    }

    String& operator+=(const String& s) {
        this->append(s);
        return *this;
    }

    /**
     * Swap contents between this and other. This function is guaranteed
     * to never fail or throw.
     */
    void swap(String& other);

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
