//
// Created by wangrl on 21-4-9.
//

#pragma once

#include <cstdint>

class IRenderableAudio {
public:
    virtual ~IRenderableAudio() = default;
    virtual void renderAudio(float* audioData, int32_t numFrames) = 0;
};
