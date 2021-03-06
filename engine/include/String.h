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

/**
 * The StrAppend... method will write into the provided buffer, assuming it is large enough.
 * Each method has an associated const (e.g. StrAppendU32MaxSize) which will be the largest
 * value needed for that method's buffer.
 *
 * char storage[StrAppendU32MaxSize];
 * StrAppendU32(storage, value);
 *
 * Note: none of the StrAppend... methods write a terminating 0 to their buffer. Instead, the
 * method return the ptr to the end of the written part of the buffer. This can be used
 * to compute the length, and/or know where to write a 0 if that is desired.
 *
 * char storage[StrAppendU32MaxSize + 1];
 * char* stop = StrAppendU32(storage, value);
 * size_t len = stop - storage;
 * *stop = 0;   // valid, since storage was 1 byte larger than the max.
 */
static constexpr int StrAppendU32MaxSize = 10;
static constexpr int StrAppendS32MaxSize = StrAppendU32MaxSize + 1;
static constexpr int StrAppendU64MaxSize = 20;
static constexpr int StrAppendS64MaxSize = StrAppendU64MaxSize + 1;

/**
 * Floats have at most 8 significant digits, so we limit our %g to that.
 * However, the total string could be 15 characters: -1.2345678e-005
 *
 * In theory we should only expect up to 2 digits for the exponent, but on
 * some platforms we have 3 (as in the example above).
 */
static constexpr int StrAppendFloatMaxSize = 15;

char* StrAppendU32(char buffer[], uint32_t);

char* StrAppendS32(char buffer[], int32_t);

char* StrAppendS64(char buffer[], int64_t, int minDigits);

char* StrAppendU64(char buffer[], uint64_t, int minDigits);

/**
 * Write the scalar in decimal format into buffer, and return a pointer to
 *  the next char after the last one written. Note: a terminating 0 is not
 *  written into buffer, which must be at least StrAppendFloatMaxSize.
 *  Thus if the caller wants to add a 0 at the end, buffer must be at least
 *  StrAppendScalarMaxSize + 1 bytes large.
 */
char* StrAppendFloat(char buffer[], float);


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

    void insertU32(size_t offset, uint32_t value);

    void insertS64(size_t offset, int64_t value, int minDigits = 0);

    void insertU64(size_t offset, uint64_t value, int minDigits = 0);

    void insertFloat(size_t offset, float);

    void append(const String& str) {
        this->insert((size_t) -1, str);
    }

    void append(const char text[]) {
        this->insert((size_t) -1, text);
    }

    void append(const char text[], size_t len) {
        this->insert((size_t) -1, text, len);
    }

    void appendS32(int32_t value) {
        this->insertS32((size_t) -1, value);
    }

    void appendU32(uint32_t value) {
        this->insertU32((size_t) -1, value);
    }

    void appendS64(int64_t value, int minDigits = 0) {
        this->insertS64((size_t) -1, value, minDigits);
    }

    void appendU64(uint64_t value, int minDigits = 0) {
        this->insertU64((size_t) -1, value, minDigits);
    }

    void appendFloat(float value) {
        this->insertFloat((size_t) -1, value);
    }

    void prepend(const String& str) {
        this->insert(0, str);
    }

    void prepend(const char text[]) {
        this->insert(0, text);
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
