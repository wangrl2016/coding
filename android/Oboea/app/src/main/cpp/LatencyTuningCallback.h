//
// Created by wangrl on 21-4-9.
//

#pragma once

#include <memory>
#include "oboe/LatencyTuner.h"
#include "DefaultDataCallback.h"

class LatencyTuningCallback : public DefaultDataCallback {
public:
    LatencyTuningCallback() : DefaultDataCallback() {}

    /**
     * Every time the playback stream requires data this method will be called.
     *
     * @param oboeStream the audio stream which is requesting data, this is the mPlayStream object
     * @param audioData an empty buffer into which we can write out audio data
     * @param numFrames the number of audio frames which are required
     * @return Either oboe::DataCallbackResult::Continue if the stream should continue requesting data
     * or oboe::DataCallbackResult::Stop if the stream should stop.
     */
    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream,
                                          void *audioData, int32_t numFrames) override;

    void setBufferTuneEnabled(bool enabled) {
        mBufferTuneEnabled = enabled;
    }

    void useStream(std::shared_ptr<oboe::AudioStream> stream);

private:
    bool mBufferTuneEnabled = true;

    // This will be used to automatically tune the buffer size of the stream, obtaining
    // optimal latency.
    std::unique_ptr<oboe::LatencyTuner> mLatencyTuner;

    oboe::AudioStream *mStream = nullptr;
};
