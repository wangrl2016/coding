//
// Created by wangrl on 21-4-15.
//

#pragma once

#include <android/asset_manager.h>
#include "IRenderableAudio.h"

class Player : public IRenderableAudio {
public:
    Player(std::shared_ptr<DataSource> source) : mSource(source) {};

    void renderAudio(float *targetData, int32_t numFrames);

private:
    std::shared_ptr<DataSource> mSource;
};
