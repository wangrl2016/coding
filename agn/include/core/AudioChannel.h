//
// Created by wangrl2016 on 2021/5/6.
//

#pragma once

#include <memory>
#include "include/AudioArray.h"

namespace agn {
    // An AudioChannel represents a buffer of non-interleaved floating-point audio samples.
    // The PCM samples are normally assumed to be in a nominal range -1.0 -> +1.0.
    class AudioChannel {
    public:
        // non-copyable
        AudioChannel(const AudioChannel&) = delete;

        AudioChannel(float* storage, int length) : mLength(length),
                                                   mRawPointer(storage), mSilent(false) {}

        // Manage storage for us.
        explicit AudioChannel(int length) : mLength(length), mSilent(true) {
            mMemBuffer.reset(new AudioFloatArray(length));
        }

        // An empty audio channel -- must call set() before it's useful.
        AudioChannel() : mLength(0), mSilent(true) {
        }

        // Redefine the memory for this channel.
        // Storage represents external memory not managed by this object.
        void set(float* storage, int length) {
            mMemBuffer.reset(); // clean up managed storage
            mRawPointer = storage;
            mLength = length;
            mSilent = false;
        }

        // How many sample-frames do we contains?
        int length() const {
            return mLength;
        }

        // resizeSmaller() can only be called with a new length <= the current length.
        // The data stored in the bus will remain undisturbed.
        void resizeSmaller(int newLength);

        // Direct access to PCM sample data. Non-const accessor clears silent flag.
        float* mutableData() {
            clearSilentFlag();
            return const_cast<float *>(data());
        }

        const float* data() const {
            if (mRawPointer)
                return mRawPointer;
            else if (mMemBuffer)
                return mMemBuffer->data();
            else
                return nullptr;
        }

        // Zeros out all sample values in buffer.
        void zero() {
            if (mSilent)
                return;
            mSilent = true;
            if (mMemBuffer)
                mMemBuffer->zero();
            else if (mRawPointer)
                memset(mRawPointer, 0, sizeof(float) * mLength);
        }

        // Clears the silent flag.
        void clearSilentFlag() {
            mSilent = false;
        }

        bool isSilent() const {
            return mSilent;
        }

        // Scales all samples by the same amount.
        void scale(float scale);

        // A simple memcpy() from the source channel.
        void copyFrom(const AudioChannel* sourceChannel);

        // Copies the given range from the source channel.
        void copyFromRange(const AudioChannel* sourceChannel, int startFrame, int endFrame);

        // Sums (with unity gain) from the source channel.
        void sumFrom(const AudioChannel* sourceChannels);

        // Returns maximum absolute value (useful for normalization).
        float maxAbsValue() const;
    private:
        int mLength = 0;
        // 二选一
        float* mRawPointer = nullptr;
        std::unique_ptr<AudioFloatArray> mMemBuffer;
        bool mSilent = true;
    };
}
