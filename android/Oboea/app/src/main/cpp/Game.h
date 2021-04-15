//
// Created by wangrl on 21-4-13.
//

#pragma once

#include <future>
#include <android/asset_manager.h>
#include <oboe/Oboe.h>

#include "AAssetDataSource.h"
#include "Mixer.h"

#include "Player.h"

using namespace oboe;

enum class GameState {
    Loading,
    Playing,
    FailedToLoad
};

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

    bool setupAudioSources();

private:
    AAssetManager &mAssetManager;
    std::shared_ptr<AudioStream> mAudioStream;
    std::unique_ptr<Player> mClap;
    Mixer mMixer;

    std::unique_ptr<float[]> mConversionBuffer{nullptr};    // for float -> int16 conversion
    std::atomic<GameState> mGameState{GameState::Loading};
    std::future<void> mLoadingResult;
};

