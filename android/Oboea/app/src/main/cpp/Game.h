//
// Created by wangrl on 21-4-13.
//

#pragma once

#include <future>
#include <android/asset_manager.h>

#include "oboe/Oboe.h"

using namespace oboe;

class Game : public AudioStreamCallback {
public:
    explicit Game(AAssetManager &);

    void start();

    void stop();

    // Inherited from oboe::AudioStreamDataCallback.
    DataCallbackResult onAudioReady(AudioStream *oboeStream,
                                    void *audioData, int32_t numFrames) override;

private:
    void load();

    bool openStream();

private:
    AAssetManager &mAssetManager;
    std::shared_ptr<AudioStream> mAudioStream;

    std::unique_ptr<float[]> mConversionBuffer{nullptr};    // for float -> int16 conversion

    std::future<void> mLoadingResult;
};

