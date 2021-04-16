//
// Created by wangrl on 21-4-13.
//

#pragma once

#include <future>
#include <android/asset_manager.h>
#include <oboe/Oboe.h>

#include "AAssetDataSource.h"
#include "LockFreeQueue.h"
#include "Mixer.h"

#include "Player.h"
#include "UtilityFunctions.h"

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

    void onSurfaceCreated();

    void onSurfaceDestroyed();

    void onSurfaceChanged(int widthInPixels, int heightInPixels);

    void tick();

    void tap(int64_t eventTimeAsUptime);

    // Inherited from oboe::AudioStreamDataCallback.
    DataCallbackResult onAudioReady(AudioStream *oboeStream,
                                    void *audioData, int32_t numFrames) override;

    // Inherited from oboe::AudioStreamErrorCallback.
    void onErrorAfterClose(AudioStream *oboeStream, Result error) override;

private:
    void load();

    bool openStream();

    bool setupAudioSources();

    void scheduleSongEvents();

    TapResult getTapResult(int64_t tapTimeInMillis, int64_t tapWindowInMillis);

private:
    AAssetManager &mAssetManager;
    std::shared_ptr<AudioStream> mAudioStream;
    std::unique_ptr<Player> mClap;
    std::unique_ptr<Player> mBackingTrack;
    Mixer mMixer;

    std::atomic<int64_t> mCurrentFrame { 0 };

    std::unique_ptr<float[]> mConversionBuffer{nullptr};    // for float -> int16 conversion

    LockFreeQueue<int64_t, kMaxQueueItems> mClapEvents;
    LockFreeQueue<int64_t, kMaxQueueItems> mClapWindows;
    LockFreeQueue<TapResult, kMaxQueueItems> mUiEvents;

    std::atomic<GameState> mGameState{GameState::Loading};
    std::future<void> mLoadingResult;

    std::atomic<int64_t> mSongPositionMs { 0 };
    std::atomic<int64_t> mLastUpdateTime { 0 };
};

