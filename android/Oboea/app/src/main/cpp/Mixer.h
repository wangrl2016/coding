//
// Created by wangrl on 21-4-13.
//

#pragma once

#include <array>
#include "IRenderableAudio.h"

// Temporary buffer is used for mixing.
constexpr int32_t kBufferSize = 192 * 10;
constexpr uint8_t kMaxTracks = 100;

/**
 * A Mixer object which sums the output from multiple tracks into a single output. The number of
 * input channels on each track must match the number of output channels (default 1=mono). This can
 * be changed by calling setChannelCount.
 * The inputs to the mixer are not owned by the mixer, they should not be delete while rendering.
 */
class Mixer : public IRenderableAudio {
public:
    void renderAudio(float *audioData, int32_t numFrames) {
        // Zero out the incoming container array.
        memset(audioData, 0, sizeof(float) * numFrames * mChannelCount);

        for (int i = 0; i < mNextFreeTrackIndex; ++i) {
            mTracks[i]->renderAudio(mixingBuffer, numFrames);

            for (int j = 0; j < numFrames * mChannelCount; ++j) {
                audioData[j] += mixingBuffer[j];
            }
        }
    }

    void addTrack(IRenderableAudio *renderer) {
        mTracks[mNextFreeTrackIndex++] = renderer;
    }

    void setChannelCount(int32_t channelCount) {
        mChannelCount = channelCount;
    }

private:
    float mixingBuffer[kBufferSize];
    std::array<IRenderableAudio *, kMaxTracks> mTracks;
    uint8_t mNextFreeTrackIndex = 0;
    // Default to mono.
    int32_t mChannelCount = 1;
};