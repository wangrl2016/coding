//
// Created by wangrl on 21-4-9.
//

#pragma once

#include <vector>
#include <unistd.h>
#include <sys/syscall.h>

#include "oboe/AudioStreamCallback.h"

#include "IRenderableAudio.h"
#include "LogMacros.h"

class DefaultDataCallback : public oboe::AudioStreamDataCallback {
public:
    DefaultDataCallback() {}

    virtual ~DefaultDataCallback() = default;

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *audioStream,
                                          void *audioData, int32_t numFrames) override {
        if (mIsThreadAffinityEnabled && !mIsThreadAffinitySet) {
            setThreadAffinity();
            mIsThreadAffinitySet = true;
        }

        float *outputBuffer = static_cast<float *>(audioData);

        std::shared_ptr<IRenderableAudio> localRenderable = mRenderable;
        if (!localRenderable) {
            LOGE("Renderable source not set");
            return oboe::DataCallbackResult::Stop;
        }
        localRenderable->renderAudio(outputBuffer, numFrames);
        return oboe::DataCallbackResult::Continue;
    }

    void setSource(std::shared_ptr<IRenderableAudio> renderable) {
        mRenderable = renderable;
    }

    /**
     * Reset the callback to its initial state.
     */
    void reset() {
        mIsThreadAffinitySet = false;
    }

    std::shared_ptr<IRenderableAudio> getSource() {
        return mRenderable;
    }

    /**
     * Set the CPU IDs to bind the audio callback thread to.
     *
     * @param cpuIds - the CPUIDs to bind to
     */
    void setCpuIds(std::vector<int> cpuIds) {
        mCpuIds = std::move(cpuIds);
    }

    /**
     * Enable or disable binding the audio callback thread to specific CPU cores. The CPU core IDs
     * can be specified using @see setCpuIds. If no CPU IDs are specified the initial core which the
     * audio thread is called on will be used.
     *
     * @param isEnabled - whether the audio callback thread should be bound the specific CPU core(s)
     */
    void setThreadAffinityEnabled(bool isEnabled) {
        mIsThreadAffinityEnabled = isEnabled;
        LOGD("Thread affinity enabled: %s", (isEnabled) ? "true" : "false");
    }

private:
    std::shared_ptr<IRenderableAudio> mRenderable;
    std::vector<int> mCpuIds;   // IDs of CPU cores which the audio callback should be bound to
    std::atomic<bool> mIsThreadAffinityEnabled{false};
    std::atomic<bool> mIsThreadAffinitySet{false};

    /**
     * Sets the thread affinity for the current thread to mCpuIds. This can be useful to call on
     * the audio thread to avoid underruns caused by CPU core implementation to slower CPU cores.
     */
    void setThreadAffinity() {
        pid_t currentThreadId = gettid();
        cpu_set_t cpuSet;
        CPU_ZERO(&cpuSet);

        // If the callback cpu ids aren't specified then bind to the current cpu.
        if (mCpuIds.empty()) {
            int currentCpuId = sched_getcpu();
            LOGD("Binding to current CPU ID %d", currentCpuId);
            CPU_SET(currentCpuId, &cpuSet);
        } else {
            LOGD("Binding to %d CPU IDs", static_cast<int>(mCpuIds.size()));
            for (size_t i = 0; i < mCpuIds.size(); i++) {
                int cpuId = mCpuIds.at(i);
                LOGD("CPU ID %d added to cores set", cpuId);
                CPU_SET(cpuId, &cpuSet);
            }
        }

        int result = sched_setaffinity(currentThreadId, sizeof(cpu_set_t), &cpuSet);
        if (result == 0) {
            LOGV("Thread affinity set");
        } else {
            LOGW("Error setting thread affinity. Error no: %d", result);
        }
        mIsThreadAffinitySet = true;
    }
};
