//
// Created by wangrl on 2021/3/29.
//

#pragma once

/**
 * WeakRefCnt is the base class for objects that may be shared by multiple
 * objects. When an existing strong owner wants to share a reference, it calls
 * ref(). When a strong owner wants to release its reference, it calls
 * unref(). When the shared object's strong reference count goes to zero as
 * the result of an unref() call, its (virtual) weak
 */

class WeakRefCnt : public RefCntBase {
public:
    /**
     * Default construct, initializing the reference counts to 1.
     * The strong references collectively hold one weak reference. When the
     * strong reference count goes to zero, the collectively held weak
     * reference is released.
     */
    WeakRefCnt() : RefCntBase(), fWeakCnt(1) {}

private:
    mutable std::atomic<int32_t> fWeakCnt;

    using INHERITED = RefCntBase;

};
