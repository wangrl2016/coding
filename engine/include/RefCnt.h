//
// Created by wangrl on 2021/3/28.
//

#pragma once

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
    RefCntBase() : fRefCnt(1) {}

private:
    mutable std::atomic<int32_t> fRefCnt;
};

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
    SharedPtr(SharedPtr<T>&& that) : fPtr(that.release()) {}

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
        if (this != that) {
            this->reset(SafeRef(that.get));
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
    SharedPtr<T>& operator=(SharedPtr<T>&& that) {
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
