//
// Created by wangrl on 21-4-9.
//

#pragma once

#include "oboe/AudioStreamCallback.h"

#include "IRenderableAudio.h"

class DefaultDataCallback : public oboe::AudioStreamDataCallback {
public:
    DefaultDataCallback() {}

    virtual ~DefaultDataCallback() = default;
};
