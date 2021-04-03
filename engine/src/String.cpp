//
// Created by wangrl on 2021/3/21.
//

#include <cassert>
#include "include/String.h"

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

String::~String() {
    this->validate();
}

#define SizeOfRec() (g)

SharedPtr<String::Rec> String::Rec::Make(const char text[], size_t len) {
    if (0 == len) {
        return SharedPtr<String::Rec>(const_cast<Rec*>(&gEmptyRec));
    }
    SafeMath safe;
    // We store a 32bit version of the length.
    uint32_t stringLen = safe.castTo<uint32_t>(len);
    // Add SizeOfRec() for our overhead and 1 for nul-termination.
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
