//
// Created by wangrl on 21-4-9.
//

#pragma once

#include <vector>
#include <unistd.h>
#include <sys/syscall.h>

#include "oboe/AudioStreamCallback.h"

#include "IRenderableAudio.h"

class DefaultDataCallback : public oboe::AudioStreamDataCallback {
public:
    DefaultDataCallback() {}

    virtual ~DefaultDataCallback() = default;

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *audioStream,
                                          void *audioData, int32_t numFrames) override {

    }

private:
    std::shared_ptr<IRenderableAudio> mRenderable;
    std::vector<int> mCpuIds;   // IDs of CPU cores which the audio callback should be bound to
    std::atomic<bool> mIsThreadAffinityEnabled{false};
    std::atomic<bool> mIsThreadAffinitySet{false};

    void setThreadAffinity() {
        pid_t currentThreadId = gettid();
        cpu_set_t cpuSet;
        CPU_ZERO(&cpuSet);

        // If the callback cpu ids aren't specified then bind to the current cpu.
    }
};
