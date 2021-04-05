//
// Created by wangrl on 2021/3/21.
//

#include <cassert>
#include "include/String.h"
#include "include/Malloc.h"

// Number of bytes (on the stack) to receive the printf result.
static const size_t kBufferSize = 1024;

struct StringBuffer {
    char* fText;
    int fLength;
};

bool StrEndsWith(const char string[], const char suffixStr[]) {
    assert(string);
    assert(suffixStr);
    size_t strLen = strlen(string);
    size_t suffixLen = strlen(suffixStr);
    return strLen >= suffixLen &&
           !strncmp(string + strLen - suffixLen, suffixStr, suffixLen);
}

bool StrEndsWith(const char string[], const char suffixChar) {
    assert(string);
    size_t strLen = strlen(string);
    if (0 == strLen) {
        return false;
    } else {
        return (suffixChar == string[strLen - 1]);
    }
}

char* StrAppendU32(char string[], uint32_t dec) {
    char* start = string;
    char buffer[StrAppendU32MaxSize];
    char* p = buffer + sizeof(buffer);
    do {
        *--p = ToU8('0' + dec % 10);
        dec /= 10;
    } while (dec != 0);
    assert(p >= buffer);
    char* stop = buffer + sizeof(buffer);
    while (p < stop) {
        *string++ = *p++;
    }
    assert(string - start <= StrAppendU32MaxSize);
    return string;
}

char* StrAppendS32(char string[], int32_t dec) {
    uint32_t uDec = dec;
    if (dec < 0) {
        *string++ = '-';
        // uDec = -uDec, but silences some warnings that are trying to be helpful.
        uDec = ~uDec + 1;
    }
    return StrAppendU32(string, uDec);
}

const String::Rec String::gEmptyRec(0, 0);

String::String() : fRec(const_cast<Rec*>(&gEmptyRec)) {}

String::String(size_t len) {
    fRec = Rec::Make(nullptr, len);
}

String::String(const char text[]) {
    size_t len = text ? strlen(text) : 0;
    fRec = Rec::Make(text, len);
}

String::String(const char text[], size_t len) {
    fRec = Rec::Make(text, len);
}

String::~String() {
    this->validate();
}

bool String::equals(const String& src) const {
    return fRec == src.fRec || this->equals(src.c_str(), src.size());
}

bool String::equals(const char text[]) const {
    return this->equals(text, text ? strlen(text) : 0);
}

bool String::equals(const char text[], size_t len) const {
    assert(len == 0 || text != nullptr);
    return fRec->fLength == len && !CarefulMemcmp(fRec->data(), text, len);
}

static uint32_t TrimSizeToU32(size_t value) {
    if (sizeof(size_t) > sizeof(uint32_t)) {
        if (value > UINT32_MAX) {
            value = UINT32_MAX;
        }
    }
    return (uint32_t) value;
}

void String::set(const char text[]) {
    this->set(text, text ? strlen(text) : 0);
}

void String::set(const char text[], size_t len) {
    len = TrimSizeToU32(len);
    if (0 == len) {
        this->reset();
    } else if (fRec->unique() && ((len >> 2u) <= (fRec->fLength >> 2u))) {
        // Use less of the buffer we have without allocating a smaller one.
        // 沿用原来的内存
        char* p = this->writable_str();
        if (text) {
            memcpy(p, text, len);
        }
        p[len] = '\0';
        fRec->fLength = ToU32(len);
    } else {
        String tmp(text, len);
        this->swap(tmp);
    }
}

void String::insert(size_t offset, const char text[]) {
    this->insert(offset, text, text ? strlen(text) : 0);
}

static size_t CheckAdd32(size_t base, size_t extra) {
    assert(base <= UINT32_MAX);
    if (sizeof(size_t) > sizeof(uint32_t)) {
        if (base + extra > UINT32_MAX) {
            extra = UINT32_MAX - base;
        }
    }
    return extra;
}

void String::insert(size_t offset, const char text[], size_t len) {
    if (len) {
        size_t length = fRec->fLength;
        if (offset > length) {
            offset = length;
        }

        // Check if length + len exceed 32bits, we trim len
        len = CheckAdd32(length, len);
        if (0 == len) {
            return;
        }

        /**
         * If we're the only owner, and we have room in our allocation for the insert,
         * do it in place, rather than allocating a new buffer.
         *
         * To know we have room, compare the allocated sizes
         * beforeAlloc = Align4(length + 1)
         * afterAlloc = Align4(length + 1 + len)
         * but Align4(x) is (x + 3) >> 2 << 2
         * which is equivalent for testing to (length + 1 + 3) >> 2 == (length + 1 + 3 + len) >> 2
         * and we can then eliminate the +1+3 since that doesn't affect the answer.
         */
        if (fRec->unique() && (length >> 2u) == ((length + len) >> 2u)) {
            char* dst = this->writable_str();
            if (offset < length) {
                memmove(dst + offset + len, dst + offset, length - offset);
            }
            memcpy(dst + offset, text, len);
            dst[length + len] = 0;
            fRec->fLength = ToU32(length + len);
        } else {
            /**
             * Seems we shoulduse realloc here, since that is safe if it fails
             * (we have the original data), and might be faster than alloc/copy/free.
             */
            String tmp(fRec->fLength + len);
            char* dst = tmp.writable_str();
            if (offset > 0) {
                memcpy(dst, fRec->data(), offset);
            }
            memcpy(dst + offset, text, len);
            if (offset < fRec->fLength) {
                memcpy(dst + offset + len, fRec->data() + offset,
                       fRec->fLength - offset);
            }
            this->swap(tmp);
        }
    }
}

String& String::operator=(const String& src) {
    this->validate();
    fRec = src.fRec;
    return *this;
}

void String::resize(size_t len) {
    len = TrimSizeToU32(len);
    if (0 == len) {
        this->reset();
    } else if (fRec->unique() && ((len >> 2u) <= (fRec->fLength >> 2u))) {
        // Use less of the buffer we have without allocating a smaller one.
        char* p = this->writable_str();
        p[len] = '\0';
        fRec->fLength = ToU32(len);
    } else {
        String newString(len);
        char* dest = newString.writable_str();
        int copyLen = std::min<uint32_t>(len, this->size());
        memcpy(dest, this->c_str(), copyLen);
        dest[copyLen] = '\0';
        this->swap(newString);
    }
}

void String::reset() {
    this->validate();
    fRec.reset(const_cast<Rec*>(&gEmptyRec));
}

char* String::writable_str() {
    this->validate();

    if (fRec->fLength) {
        if (!fRec->unique()) {
            fRec = Rec::Make(fRec->data(), fRec->fLength);
        }
    }
    return fRec->data();
}

String::String(const String& src) : fRec(src.validate().fRec) {}


void String::swap(String& other) {
    this->validate();
    other.validate();
    std::swap(fRec, other.fRec);
}

void String::insertS32(size_t offset, int32_t value) {
    char buffer[StrAppendS32MaxSize];
    char* stop = StrAppendS32(buffer, value);
    this->insert(offset, buffer, stop - buffer);
}

#define SizeOfRec() (gEmptyRec.data() - (const char*)&gEmptyRec)

SharedPtr<String::Rec> String::Rec::Make(const char text[], size_t len) {
    if (0 == len) {
        return SharedPtr<String::Rec>(const_cast<Rec*>(&gEmptyRec));
    }
    SafeMath safe;
    // We store a 32bit version of the length.
    auto stringLen = safe.castTo<uint32_t>(len);
    // Add SizeOfRec() for our overhead and 1 for nul-termination.
    size_t allocationSize = safe.add(len, SizeOfRec() + sizeof(char));
    // Align up to a multiple of 4.
    allocationSize = safe.alignUp(allocationSize, 4);
    assert(safe.ok());

    void* storage = ::operator new(allocationSize);
    SharedPtr<Rec> rec(new(storage) Rec(stringLen, 1));
    if (text) {
        memcpy(rec->data(), text, len);
    }
    rec->data()[len] = 0;
    return rec;
}

void String::Rec::ref() const {
    if (this == &String::gEmptyRec) {
        return;
    }
    this->fRefCnt.fetch_add(+1, std::memory_order_relaxed);
}

void String::Rec::unref() const {
    if (this == &String::gEmptyRec) {
        return;
    }
    int32_t oldRefCnt = this->fRefCnt.fetch_add(-1, std::memory_order_acq_rel);
    assert(oldRefCnt);
    if (1 == oldRefCnt) {
        delete this;
    }
}

bool String::Rec::unique() const {
    return fRefCnt.load(std::memory_order_acquire) == 1;
}
