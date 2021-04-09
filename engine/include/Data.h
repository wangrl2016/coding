//
// Created by wangrl on 2021/4/8.
//

#pragma once

#include <cstdio>
#include "include/RefCnt.h"

class Data final : public NVRefCnt<Data> {
    /**
     * @return the number of bytes stored
     */
    size_t size() const { return fSize; }

    bool isEmpty() const { return 0 == fSize; }

    /**
     * @return the ptr to the data
     */
    const void* data() const { return fPtr; }

    /**
     * Like data()
     */
    const uint8_t* bytes() const {
        return reinterpret_cast<const uint8_t*>(fPtr);
    }

    /**
     * USE WITH CAUTION.
     *
     * This call will assert that the refCnt is 1,as a precaution modifying the
     * contents when another client/thread has access to the data.
     */
    void* writable_data() {
        if (fSize) {
            // Only assert we're unique if we're not empty.
            assert(this->unique());
        }
        return const_cast<void*>(fPtr);
    }

    /**
     * Helper to copy a range of the data into a caller-provided buffer.
     * Returns the actual number of bytes copied, after clamping offset and
     * length to the size of the data. If buffer is NULL, it is ignored, and
     * only the computed number of bytes is returned.
     */
    size_t copyRange(size_t offset, size_t length, void* buffer) const;

    /**
     * Returns true if these two objects have the same length and contents,
     * effectively returning 0 == memcmp(...)
     */
    bool equals(const Data* other) const;

    /**
     * Function that, if provided, will be called when the Data goes out
     * of scope, allowing for custom allocation/freeing of the data's contents.
     */
    typedef void (* ReleaseProc)(const void* ptr, void* context);

    /**
     * Create a new dataRef by copying the specified data.
     */
    static SharedPtr<Data> MakeWithCopy(const void* data, size_t length);

    /**
     * Create a new data with uninitialized contents. The caller should call writable_data()
     * to write into the buffer, but this must be done before another ref() is made.
     */
    static SharedPtr<Data> MakeUninitialized(size_t length);

    /**
     * Create a new dataRef by copying the specified c-string
     * (a null-terminated array of bytes). The returned Data will have size()
     * equal to strlen(str) + 1. If str is nullptr, it will be treated the same as "".
     */
    static SharedPtr<Data> MakeWithCString(const char str[]);

    /**
     * Create a new dataRef, taking the ptr as is, and using the
     * ReleaseProc to free it. The proc may be nullptr.
     */
    static SharedPtr<Data> MakeWithProc(const void* ptr, size_t length, ReleaseProc proc, void* ctx);

    /**
     * Call this when the data parameter is already const and will outlive the lifetime of the
     * Data. Suitable for with const globals.
     */
    static SharedPtr<Data> MakeWithoutCopy(const void* data, size_t length) {

    }

    /**
     * Create a new dataRef from a pointer allocated by malloc. The Data object
     * takes ownership of that allocation, and will handling call free.
     */
    static SharedPtr<Data> MakeFromMalloc(const void* data, size_t length);

    /**
     * Create a new dataRef the file with the specified path.
     * If the file cannot be opened, this returns nullptr.
     */
    static SharedPtr<Data> MakeFromFileName(const char path[]);

    /**
     * Create a new dataRef from a stdio FILE.
     * This does not take ownership of the FILE, nor close it.
     * The caller is free to close the FILE at its convenience.
     * The FILE must be open for reading only.
     * Returns nullptr on failure.
     */
    static SharedPtr<Data> MakeFromFILE(FILE* f);

    /**
     * Create a new dataRef using a subset of the data in the specified src dataRef.
     */
    static SharedPtr<Data> MakeSubset(const Data* src, size_t offset, size_t length);

    /**
     * Returns a new empty dataRef (or a reference to a shared empty dataRef).
     * New or shared, the caller must see that unref() is eventually called.
     */
    static SharedPtr<Data> MakeEmpty();

private:
    const void* fPtr;
    size_t fSize;
};