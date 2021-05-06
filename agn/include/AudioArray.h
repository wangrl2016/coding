//
// Created by wangrl2016 on 2021/5/6.
//

#pragma once

namespace agn {

    template<typename T>
    class AudioArray {
    public:
        AudioArray() : mAllocation(nullptr), mAlignedData(nullptr), mSize(0) {}

        explicit AudioArray(size_t n) : mAllocation(nullptr), mAlignedData(nullptr), mSize(0) {
            allocate(n);
        }

        ~AudioArray() {
            free(mAllocation);
        }

        // It's OK to call allocate() multiple times, but data will not be copied from an initial allocation
        // if re-allocated. Allocations are zero-initialized.
        void allocate(size_t n) {
            size_t initialSize = sizeof(T) * n;
            const size_t alignment = 16;

            if (mAllocation)
                free(mAllocation);

            bool isAllocationGood = false;

            while (!isAllocationGood) {
                // Initially we try to allocate the exact size, but if it's not aligned
                // then we'll have to reallocate and from then on allocate extra.
                static size_t extraAllocationBytes = 0;
                T* allocation = static_cast<T*>(malloc(initialSize + extraAllocationBytes));
                if (!allocation) {
                    // crash
                }
                T* alignedData = alignedAddress(allocation, alignment);

                if (alignedData == allocation || extraAllocationBytes == alignment) {
                    mAllocation = allocation;
                    mAlignedData = alignedData;
                    mSize = n;
                    isAllocationGood = true;
                    zero();
                } else {
                    // Always allocate extra after the first alignment failure.
                    extraAllocationBytes = alignment;
                    free(allocation);
                }
            }
        }

        T* data() {
            return mAlignedData;
        }

        const T* data() const {
            return mAlignedData;
        }

        size_t size() const {
            return mSize;
        }

        T& at(size_t i) {
            // Note that although it is a size_t, mSize is now guaranteed to be
            // no greater than max unsigned. This guarantee is enforced in allocate().
            return data()[i];
        }

        T& operator[](size_t i) {
            return at(i);
        }

        void zero() {
            // This multiplication is made safe by the check in allocate().
            memset(this->data(), 0, sizeof(T) * this->size());
        }

        void zeroRange(unsigned start, unsigned end) {
            bool isSafe = (start <= end) && (end <= this->size());
            if (!isSafe)
                return;

            // This expression can not overflow because end - start can not be
            // greater than mSize, which is safe due to the check in allocate().
            memset(this->data() + start, 0, sizeof(T) * (end - start));
        }

        void copyToRange(const T* sourceData, unsigned start, unsigned end) {
            bool isSafe = (start <= end) && (end <= this->size());
            if (!isSafe)
                return;
            // This expression cannot overflow because end - start cannot be
            // greater than mSize, which is safe due to the check in allocate().
            memcpy(this->data() + start, sourceData, sizeof(T) * (end - start));
        }

    private:
        static T* alignedAddress(T* address, intptr_t alignment) {
            auto value = reinterpret_cast<intptr_t>(address);
            return reinterpret_cast<T*>((value + alignment - 1) & ~(alignment - 1));
        }

        // 分配内存的起始位置
        T* mAllocation;
        // 内存对齐的起始位置
        T* mAlignedData;
        size_t mSize;
    };

    typedef AudioArray<float> AudioFloatArray;
    typedef AudioArray<double> AudioDoubleArray;
}
