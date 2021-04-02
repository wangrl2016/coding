//
// Created by wangrl on 2021/3/21.
//

#include <cassert>
#include "include/String.h"

const String::Rec String::gEmptyRec(0, 0);

String::String() : fRec(const_cast<Rec*>(&gEmptyRec)) {}

String::~String() {
    this->validate();
}

SharedPtr<String::Rec> String::Rec::Make(const char text[], size_t len) {
    if (0 == len) {
        return SharedPtr<String::Rec>(const_cast<Rec*>(&gEmptyRec));
    }

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
