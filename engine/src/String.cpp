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
