//
// Created by wangrl on 21-4-9.
//

#pragma once

#include "oboe/Oboe.h"

#include "IRestartable.h"
#include "SoundGenerator.h"
#include "DefaultErrorCallback.h"
#include "LatencyTuningCallback.h"

constexpr int32_t kBufferSizeAutomatic = 0;

class HelloOboeEngine : public IRestartable {
public:
    HelloOboeEngine();

    virtual ~HelloOboeEngine() = default;

    void tap(bool isDown);

    /**
     * Open and start a stream.
     *
     * @return error or OK
     */
    oboe::Result start();

    /**
     * Stop and close the stream.
     */
    void stop();

    // From IRestartable.
    void restart() override;

    /**
     * Set the audio device which should be used for playback. Can be set to oboe:kUnspecified if
     * you want to use the default playback device (which is usually the build-in speaker if
     * no other audio devices, such as headphones, are attached).
     *
     * @param deviceId the audio device id, can be obtained through an {@link AudioDeviceInfo}
     * object using Java/JNI.
     */
    void setDeviceId(int32_t deviceId);

    void setChannelCount(int channelCount);

    void setAudioApi(oboe::AudioApi audioApi);

    void setBufferSizeInBursts(int32_t numBursts);

    /**
     * Calculate the current latency between writing a frame to the output stream and
     * the same frame being presented to the audio hardware.
     *
     * Here's how the calculation works:
     *
     * 1) Get the time a particular frame was presented to the audio hardware
     * @see AudioStream::getTimestamp
     * 2) From this extrapolate the time which the *next* audio frame written to the stream
     * will be presented
     * 3) Assume that the next audio frame is written at the current time
     * 4) currentLatency = nextFramePresentationTime - nextFrameWriteTime
     *
     * @return Output latency in milliseconds
     */
    double getCurrentOutputLatencyMillis();

    bool isLatencyDetectionSupported();

private:
    oboe::Result reopenStream();

    oboe::Result createPlaybackStream();

private:
    std::shared_ptr<oboe::AudioStream> mStream;
    std::unique_ptr<LatencyTuningCallback> mLatencyCallback;
    std::unique_ptr<DefaultErrorCallback> mErrorCallback;
    std::shared_ptr<SoundGenerator> mAudioSource;
    bool mIsLatencyDetectionSupported = false;

    int32_t mDeviceId = oboe::Unspecified;
    int32_t mChannelCount = oboe::Unspecified;
    oboe::AudioApi mAudioApi = oboe::AudioApi::Unspecified;
    std::mutex mLock;
};