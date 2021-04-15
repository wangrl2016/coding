//
// Created by wangrl on 21-4-13.
//

#include "Game.h"
#include "LogMacros.h"
#include "GameConstants.h"

Game::Game(AAssetManager &assetManager) : mAssetManager(assetManager) {}

DataCallbackResult Game::onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) {
    return DataCallbackResult::Continue;
}

void Game::load() {
    if (!openStream()) {
        mGameState = GameState::FailedToLoad;
    }

    if (!setupAudioSources()) {
        mGameState = GameState::FailedToLoad;
        return;
    }

    scheduleSongEvents();

    Result result = mAudioStream->requestStart();
    if (result != Result::OK) {
        LOGE("Failed to start stream. Error: %s", convertToText(result));
        mGameState = GameState::FailedToLoad;
        return;
    }
    mGameState = GameState::Playing;
}

void Game::start() {
    // async returns a future, we must store this future to avoid blocking. It's not sufficient
    // to store this in a local variable as its destructor will block until Game::load completes.
    mLoadingResult = std::async(&Game::load, this);
}

void Game::stop() {
    if (mAudioStream) {
        mAudioStream->stop();
        mAudioStream->close();
        mAudioStream.reset();
    }
}

bool Game::openStream() {
    // Create an audio stream.
    AudioStreamBuilder builder;
    builder.setDataCallback(this);
    builder.setErrorCallback(this);
    builder.setPerformanceMode(PerformanceMode::LowLatency);
    builder.setSharingMode(SharingMode::Exclusive);

    Result result = builder.openStream(mAudioStream);
    if (result != Result::OK) {
        LOGE("Failed to open stream. Error: %s", convertToText(result));
        return false;
    }

    if (mAudioStream->getFormat() == AudioFormat::I16) {
        mConversionBuffer = std::make_unique<float[]>(
                (size_t) mAudioStream->getBufferCapacityInFrames() *
                mAudioStream->getChannelCount());
    }

    // Reduce stream latency by setting the buffer size to a multiple of the burst size
    auto setBufferSizeResult = mAudioStream->setBufferSizeInFrames(
            mAudioStream->getFramesPerBurst() * kBufferSizeInBursts);
    if (setBufferSizeResult != Result::OK) {
        LOGW("Failed to set buffer size. Error: %s", convertToText(setBufferSizeResult.error()));
    }

    mMixer.setChannelCount(mAudioStream->getChannelCount());

    return true;
}

bool Game::setupAudioSources() {
    // Set the properties of our audio source(s) to match that of our audio stream.
    AudioProperties targetProperties{
            .channelCount = mAudioStream->getChannelCount(),
            .sampleRate = mAudioStream->getSampleRate()
    };

    // Create a data source and player for the clap sound.
    std::shared_ptr<AAssetDataSource> mClapSource{
            AAssetDataSource::newFromCompressedAsset(mAssetManager, kClapFilename, targetProperties)
    };
    if (mClapSource == nullptr) {
        LOGE("Could not load source data for clap sound");
        return false;
    }
    mClap = std::make_unique<Player>(mClapSource);

    // Create a data source and player for out backing track.
    std::shared_ptr<AAssetDataSource> backingTrackSource{
            AAssetDataSource::newFromCompressedAsset(mAssetManager, kBackingTrackFilename,
                                                     targetProperties)
    };
    if (backingTrackSource == nullptr) {
        LOGE("Could not load source data for backing track");
        return false;
    }
    mBackingTrack = std::make_unique<Player>(backingTrackSource);
    mBackingTrack->setPlaying(true);
    mBackingTrack->setLooping(true);

    // Add both players to a mixer.
    mMixer.addTrack(mClap.get());
    mMixer.addTrack(mBackingTrack.get());

    return true;
}

void Game::scheduleSongEvents() {
    for (auto t : kClapEvents) mClapEvents.push(t);
    for (auto t : kClapWindows) mClapWindows.push(t);
}

void Game::onErrorAfterClose(AudioStream *oboeStream, Result error) {
    LOGE("The audio stream was closed, please restart the game. Error: %s", convertToText(error));
}

