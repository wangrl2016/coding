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
    return fRec->fLength == len && !CarefulMemcpy(fRec->data(), text, len);
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
