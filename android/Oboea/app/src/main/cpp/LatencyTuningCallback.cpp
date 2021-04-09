//
// Created by wangrl on 21-4-9.
//

#include <android/trace.h>
#include "LatencyTuningCallback.h"

oboe::DataCallbackResult
LatencyTuningCallback::onAudioReady(oboe::AudioStream *oboeStream, void *audioData,
                                    int32_t numFrames) {
    if (oboeStream != mStream) {
        mStream = oboeStream;
        mLatencyTuner = std::make_unique<oboe::LatencyTuner>(*oboeStream);
    }

    if (mBufferTuneEnabled
        && mLatencyTuner
        && oboeStream->getAudioApi() == oboe::AudioApi::AAudio) {
        mLatencyTuner->tune();
    }
    auto underrunCountResult = oboeStream->getXRunCount();
    int bufferSize = oboeStream->getBufferSizeInFrames();

    auto result = DefaultDataCallback::onAudioReady(oboeStream, audioData, numFrames);
    return result;
}
