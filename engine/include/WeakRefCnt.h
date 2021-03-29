//
// Created by wangrl on 2021/3/29.
//

#pragma once

#include "Types.h"

/**
 * WeakRefCnt is the base class for objects that may be shared by multiple
 * objects. When an existing strong owner wants to share a reference, it calls
 * ref(). When a strong owner wants to release its reference, it calls
 * unref(). When the shared object's strong reference count goes to zero as
 * the result of an unref() call, its (virtual) weakDispose() method is called.
 * It is an error for the destructor to be called explicitly (or via the
 * object going out of scope on the stack or calling delete) if
 * getRefCnt() > 1.
 *
 * In addition to strong ownership, an owner may instead obtain a weak
 * reference by calling weakRef(). A call to weakRef() must be balanced by a
 * called to weakUnref(). To obtain a strong reference from a weak reference,
 * call tryRef(). If tryRef() returns true, the owner's pinter is now also
 * a strong reference on which unref() must be called. Note that this does not
 * affect the original weak reference, weakUnref() must still be called. When
 * the weak reference count goes to zero, the object is delete. While the
 * weak reference count is positive and the strong reference count is zero the
 * object still exists,but will be in the disposed state. It is up to the object
 * to define what this means.
 *
 * Note that a strong reference implicitly implies a weak reference. As a result,
 * it is allowable for the owner of a strong ref to call tryRef().
 * This will have the same effect as calling ref(), but may be more expensive.
 *
 * tryRef()/unref()对强计数进行增减
 * weakRef()/weakUnref()对弱计数进行增减
 *
 * Example:
 *
 * WeakRefCnt myRef = strongRef.weakRef();
 * ... // strongRef.unref() may or may not ba called.
 * if (myRef.tryRef()) {
 *     // use myRef
 *     myRef.unref();
 * } else {
 *     // myRef is in the disposed state
 * }
 * myRef.weakUnref();
 */

class WeakRefCnt : public RefCntBase {
public:
    /**
     * Default construct, initializing the reference counts to 1.
     * The strong references collectively hold one weak reference. When the
     * strong reference count goes to zero, the collectively held weak
     * reference is released.
     */
    WeakRefCnt() : RefCntBase(), fWeakCnt(1) {
        PRINT_FUNC();
    }

    /**
     * Destruct, asserting that the weak reference count is 1.
     */
    ~WeakRefCnt() override {
        assert(getWeakCnt() == 1);
        fWeakCnt.store(0, std::memory_order_relaxed);
        PRINT_FUNC();
    }

    /**
     * @return Return the weak reference count.
     */
    int32_t getWeakCnt() const {
        PRINT_FUNC();
        return fWeakCnt.load(std::memory_order_relaxed);
    }

    /**
     * Creates a strong reference from a weak reference, if possible. The
     * caller must already be an owner. If try_ref() return true the owner
     * is in possession of a additional strong reference. Both the original
     * reference and nre reference must be properly unreferenced. If try_ref()
     * return false, no strong reference could be created and the owner's
     * reference is in the same state as before the call.
     */
    bool tryRef() const {
        PRINT_FUNC();
        // Acquire barrier (L/SL), if not provided above.
        // Prevents subsequent code from happening before the increment.
        return atomicConditionalAcquireStrongRef() != 0;
    }

    /**
     * Increment the weak reference count. Must be balanced by a call to weak_unref().
     */
    void weakRef() const {
        PRINT_FUNC();
        assert(getRefCnt() > 0);
        assert(getWeakCnt() > 0);
        // No barrier required.
        (void) fWeakCnt.fetch_add(+1, std::memory_order_relaxed);
    }

    /**
     * Decrement the weak reference count. If the weak reference count is 1
     * before the decrement, then call delete on the object. Note that if this
     * is the case, then the object needs to have been allocated view new, and
     * not on the stack.
     */
    void weakUnref() const {
        PRINT_FUNC();
        assert(getWeakCnt() > 0);
        // A release here acts in place of all releases we "should" have been doing in ref().
        if (1 == fWeakCnt.fetch_add(-1, std::memory_order_acq_rel)) {
            // Like try_ref(), the acquire is only needed on success, to make sure
            // code in internalDispose() doesn't happen before the decrement.
            // So our destructor won't complain.
            // debug
            fWeakCnt.store(1, std::memory_order_relaxed);
            this->INHERITED::internalDispose();
        }
    }

    /**
     * Returns true if there are no strong references to the object. When this
     * is the case all future calls to tryRef() will return false.
     */
    bool weakExpired() const {
        PRINT_FUNC();
        return fRefCnt.load(std::memory_order_relaxed) == 0;
    }

protected:
    /**
     * Called when the strong reference count goes to zero. This allows the
     * object to free any resources it may be holding. Weak references may
     * still exist and their level of allowed access to the object is defined
     * by the object's class.
     */
    virtual void weakDispose() const {
        PRINT_FUNC();
    }

private:
    /**
     * Called when the strong reference count goes to zero. Call weakDispose
     * on the object and releases the implicit weak reference held
     * collectively by the strong references.
     */
    void internalDispose() const override {
        weakDispose();
        weakUnref();
    }

    /**
     * If fRefCnt is 0, return 0.
     * Otherwise increments fRefCnt, acquires, and returns the old value.
     */
    int32_t atomicConditionalAcquireStrongRef() const {
        int32_t prev = fRefCnt.load(std::memory_order_relaxed);
        do {
            if (0 == prev) {
                break;
            }
            // 将fRefCnt和prev对比，如果相等，fRefCnt设置为prev+1。
        } while (!fRefCnt.compare_exchange_weak(prev, prev + 1, std::memory_order_acquire,
                                                std::memory_order_relaxed));
        return prev;
    }

    mutable std::atomic<int32_t> fWeakCnt;

    using INHERITED = RefCntBase;
};
