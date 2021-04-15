//
// Created by wangrl on 21-4-15.
//

#pragma once

#include <memory>

#include <android/asset_manager.h>
#include "IRenderableAudio.h"
#include "DataSource.h"

class Player : public IRenderableAudio {
public:
    /**
     * Construct a new Player from the given DataSource. Players can share the same data source.
     * For example, you could play two identical sound concurrently by creating 2
     * @param source
     */
    Player(std::shared_ptr<DataSource> source) : mSource(source) {};

    void renderAudio(float *targetData, int32_t numFrames);

    void resetPlayHead() {
        mReadFrameIndex = 0;
    }

    void setPlaying(bool isPlaying) {
        mIsPlaying = isPlaying;
        resetPlayHead();
    }

    void setLooping(bool isLooping) {
        mIsLooping = isLooping;
    }

private:
    void renderSilence(float *, int32_t);

private:
    int32_t mReadFrameIndex = 0;
    std::atomic<bool> mIsPlaying{false};
    std::atomic<bool> mIsLooping{false};
    std::shared_ptr<DataSource> mSource;
};

