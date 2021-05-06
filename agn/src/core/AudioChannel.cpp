//
// Created by wangrl2016 on 2021/5/6.
//

#include <cassert>

#include "include/core/AudioChannel.h"
#include "src/internal/Assertions.h"
#include "src/internal/VectorMath.h"

namespace agn {

    void AudioChannel::resizeSmaller(int newLength) {
        assert(newLength <= mLength);
        if (newLength <= mLength)
            mLength = newLength;
    }

    void AudioChannel::scale(float scale) {
        if (isSilent())
            return;
        assert(false);
    }

    void AudioChannel::copyFrom(const AudioChannel* sourceChannel) {
        bool isSafe = (sourceChannel && sourceChannel->length() >= length());
        assert(isSafe);
        if (!isSafe)
            return;
        if (sourceChannel->isSilent()) {
            zero();
            return;
        }
        memcpy(mutableData(), sourceChannel->data(), sizeof(float) * length());
    }

    void AudioChannel::copyFromRange(const AudioChannel* sourceChannel, int startFrame, int endFrame) {
        // Check that range is safe for reading from sourceChannel.
        bool isRangeSafe = sourceChannel && startFrame < endFrame && endFrame <= sourceChannel->length();
        assert(isRangeSafe);
        if (!isRangeSafe)
            return;
        if (sourceChannel->isSilent() && isSilent())
            return;

        // Check that this channel has enough space.
        size_t rangeLength = endFrame - startFrame;
        bool isRangeLengthSafe = rangeLength <= length();
        assert(isRangeLengthSafe);
        if (!isRangeLengthSafe)
            return;

        const float* source = sourceChannel->data();
        float* destination = mutableData();
        if (sourceChannel->isSilent()) {
            if (rangeLength == length())
                zero();
            else
                memset(destination, 0, sizeof(float) * rangeLength);
        } else {
            memcpy(destination, source + startFrame, sizeof(float) * rangeLength);
        }
    }

    void AudioChannel::sumFrom(const AudioChannel* sourceChannels) {
        assert(false);
    }

    float AudioChannel::maxAbsValue() const {
        assert(false);
        return 0;
    }


}