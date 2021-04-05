//
// Created by wangrl on 2021/3/28.
//

#pragma once

#include "include/Types.h"

/**
 * RefCntBase is the base class for objects that maybe shared by multiple
 * objects. When an existing owner wants to share a reference, it calls ref().
 * When an owner wants to release its reference, it calls unref(). When the
 * shared object's reference count goes to zero as the result of an unref()
 * call, its (virtual) destructor is called. It is an error for the
 * destructor to be called explicitly (or via the object going out of scope on
 * the stack or calling delete) if getRefCnt() > 1.
 */
class RefCntBase {
public:
    /**
     * Default construct, initializing the reference count to 1.
     */
    RefCntBase() : fRefCnt(1) {
        PRINT_FUNC();
    }

    /**
     * Destruct, asserting that the reference count is 1.
     */
    virtual ~RefCntBase() {
        assert(getRefCnt() == 1);
        // Illegal value,to catch us if we reuse after delete.
        fRefCnt.store(0, std::memory_order_relaxed);
        PRINT_FUNC();
    }

    /**
     * May return true if the caller isthe only owner.
     * Ensures that all previous owner's actions are complete.
     */
    bool unique() const {
        if (1 == fRefCnt.load(std::memory_order_acquire)) {
            // This acquire barrier is only really needed if we return true. It
            // prevents code conditioned on the result of unique() from running
            // until previous owners are all totally done calling unref().
            return true;
        }
        return false;
    }

    /**
     * Increment the reference count. Must be balanced by a call to unref().
     */
    void ref() const {
        assert(getRefCnt() > 0);
        // No barrier required.
        (void) fRefCnt.fetch_add(+1, std::memory_order_relaxed);
        PRINT_FUNC();
    }

    /**
     * Decrement the reference count. If the reference count is 1 before the decrement, then
     * delete the object. Note that if this is the case, then the object needs to have been
     * allocated via new, and not on the stack.
     */
    void unref() const {
        PRINT_FUNC();
        assert(getRefCnt() > 0);
        // A release here acts in place of all releases we "should" have been doing in ref().
        if (1 == fRefCnt.fetch_add(-1, std::memory_order_acq_rel)) {
            // Like unique(), the acquire is only needed on success, to make sure
            // code in internalDispose() doesn't happen before the decrement.
            this->internalDispose();
        }
    }

    /**
     * @return the reference count. Use only for debugging
     */
    int32_t getRefCnt() const {
        return fRefCnt.load(std::memory_order_relaxed);
    }

    RefCntBase(RefCntBase&&) = delete;

    RefCntBase(const RefCntBase&) = delete;

    RefCntBase& operator=(RefCntBase&&) = delete;

    RefCntBase& operator=(const RefCntBase&) = delete;

private:

    /**
     * Called when the ref count goes to 0.
     */
    virtual void internalDispose() const {
        assert(0 == this->getRefCnt());
        // debug
        fRefCnt.store(1, std::memory_order_relaxed);
        delete this;
    }

    // The following friends are those which override internalDispose()
    // and conditionally call RefCnt::internalDispose().
    // 可以访问私有变量
    friend class WeakRefCnt;

    mutable std::atomic<int32_t> fRefCnt;
};

/**
 * Call obj->ref() and return obj. The obj must not be nullptr.
 */
template<typename T>
static inline T* Ref(T* obj) {
    assert(obj);
    obj->ref();
    return obj;
}

/**
 * Check if the argument is non-null, and if so, call obj->ref() and return obj.
 */
template<typename T>
static inline T* SafeRef(T* obj) {
    if (obj) {
        obj->ref();
    }
    return obj;
}

/**
 * Check if the argument is non-null, and if so, call obj->unref().
 */
template<typename T>
static inline void SafeUnref(T* obj) {
    if (obj) {
        obj->unref();
    }
}

template<typename Derived>
class NVRefCnt {
public:
    NVRefCnt() : fRefCnt(1) {}

    ~NVRefCnt() {
        // debug
        int rc = fRefCnt.load(std::memory_order_relaxed);
        assert(rc == 0);
    }

    /**
     * Implementation is pretty much the same as RefCntBase. All required barriers are the same:
     *    - unique() needs acquire when it returns true, and no barrier if it returns false;
     *    - ref() doesn't need any barrier;
     *    - unref() needs a release barrier, and an acquire if it's going to call delete.
     */
    bool unique() const {
        return 1 == fRefCnt.load(std::memory_order_acquire);
    }

    void ref() const {
        fRefCnt.fetch_add(+1, std::memory_order_relaxed);
    }

    void unref() const {
        if (1 == fRefCnt.fetch_add(-1, std::memory_order_acq_rel)) {
            delete (const Derived*) this;
        }
    }

    void deref() const {
        this->unref();
    }

    NVRefCnt(const NVRefCnt&) = delete;

    NVRefCnt(NVRefCnt&&) = delete;

    NVRefCnt& operator=(NVRefCnt&&) = delete;

    NVRefCnt& operator=(const NVRefCnt&) = delete;

private:
    mutable std::atomic<int32_t> fRefCnt;
};

/**
 * Shared pointer class to wrap classes that support a ref()/unref() interface.
 *
 * This can be used for classes inheriting from RefCnt, but it also works for other
 * classes that match the interface, but have different internal choices: e.g. the
 * hosted class may have its ref/unref be thread-safe, but that is not assumed/imposed
 * by SharedPtr.
 */
template<typename T>
class SharedPtr {
public:
    using element_type = T;

    constexpr SharedPtr() : fPtr(nullptr) {}

    constexpr SharedPtr(std::nullptr_t) : fPtr(nullptr) {}

    /**
     * Shares the underlying object by calling ref(), so that both the argument and the
     * newly created SharedPtr both have a reference to it.
     */
    SharedPtr(const SharedPtr<T>& that) : fPtr(SafeRef(that.get())) {}

    template<typename U, typename = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
    SharedPtr(const SharedPtr<U>& that) : fPtr(SafeRef(that.get())) {};

    /**
     * Move the underlying object from the argument to the newly create SharedPtr. Afterwards only
     * the new SharedPtr will have a reference to the object, and the argument will point to null.
     * No call to ref() or unref() will be made.
     */
    SharedPtr(SharedPtr<T>&& that) noexcept: fPtr(that.release()) {}

    template<typename U,
            typename =typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
    SharedPtr(SharedPtr<U>&& that) : fPtr(that.release()) {}

    /**
     * Adopt the bare pointer into the newly created SharedPtr.
     * No call to ref() or unref() will be made.
     */
    explicit SharedPtr(T* obj) : fPtr(obj) {}

    /**
     * Calls unref() on the underlying object pointer.
     */
    ~SharedPtr() {
        SafeUnref(fPtr);
        fPtr = nullptr;
    }

    /**
     * Shares the underlying object referenced by the argument by calling ref() on it. If this
     * SharedPtr previously had a reference to an object (i.e. not null) it will call unref() on that
     * object.
     */
    SharedPtr<T>& operator=(const SharedPtr<T>& that) {
        if (this != &that) {
            this->reset(SafeRef(that.get()));
        }
        return *this;
    }

    template<typename U,
            typename = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
    SharedPtr<T>& operator=(const SharedPtr<U>& that) {
        this->reset(SafeRef(that.get()));
        return *this;
    }

    /**
     * Move the underlying object from the argument to the SharedPtr. If the SharedPtr previously held
     * a reference to another object, unref() will be called on that object. No call to ref()
     * will be made.
     */
    SharedPtr<T>& operator=(SharedPtr<T>&& that) noexcept {
        this->reset(that.release());
        return *this;
    }

    T& operator*() const {
        assert(this->get() != nullptr);
        return *this->get();
    }

    explicit operator bool() const { return this->get() != nullptr; }

    T* get() const { return fPtr; }

    T* operator->() const { return fPtr; }

    /**
     * Adopt the new bare pointer, and call unref() on any previously held object (if not null).
     * No call to ref() will be made.
     */
    void reset(T* ptr = nullptr) {
        // Calling fPtr->unref() may call this->~() or this->reset(T*).
        T* oldPtr = fPtr;
        fPtr = ptr;
        SafeUnref(oldPtr);
    }

    T* release() {
        T* ptr = fPtr;
        fPtr = nullptr;
        return ptr;
    }

    void swap(SharedPtr<T>& that) {
        std::swap(fPtr, that.fPtr);
    }

private:
    T* fPtr;
};

template<typename T, typename... Args>
SharedPtr<T> makeSharedPtr(Args&& ... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

/**
 * Returns a SharedPtr wrapping the provided ptr AND calls ref on it (if not null).
 *
 * This is different than the semantics of the constructor for SharedPtr, which just wraps the ptr,
 * effectively "adopting" it.
 */
template<typename T>
SharedPtr<T> refSharedPtr(T* obj) {
    return SharedPtr<T>(SafeRef(obj));
}

template<typename T>
SharedPtr<T> refSharedPtr(const T* obj) {
    return SharedPtr<T>(const_cast<T*>(SafeRef(obj)));
}

template<typename T>
inline void swap(SharedPtr<T>& a, SharedPtr<T>& b) {
    a.swap(b);
}

template<typename T, typename U>
inline bool operator==(const SharedPtr<T>& a, const SharedPtr<U>& b) {
    return a.get() == b.get();
}

template<typename T>
inline bool operator==(const SharedPtr<T>& a, std::nullptr_t) {
    return !a;
}

template<typename T>
inline bool operator==(std::nullptr_t, const SharedPtr<T>& b) {
    return !b;
}

template<typename T, typename U>
inline bool operator!=(const SharedPtr<T>& a, const SharedPtr<U>& b) {
    return a.get() != b.get();
}

template<typename T>
inline bool operator!=(const SharedPtr<T>& a, std::nullptr_t) {
    return static_cast<bool>(a);
}

template<typename T>
inline bool operator!=(std::nullptr_t, const SharedPtr<T>& b) {
    return static_cast<bool>(b);
}

template<typename C, typename CT, typename T>
auto operator<<(std::basic_ostream<C, CT>& os, const SharedPtr<T>& sp) -> decltype(os << sp.get()) {
    return os << sp.get();
}
