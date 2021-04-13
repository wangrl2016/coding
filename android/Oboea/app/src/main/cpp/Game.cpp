//
// Created by wangrl on 21-4-13.
//

#include "Game.h"
#include "LogMacros.h"
#include "GameConstants.h"
#include "SampleBuffer.h"

using namespace iolib;

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
}

void Game::start() {
    // async returns a future, we must store this future to avoid blocking. It's not sufficient
    // to store this in a local variable as its destructor will block until Game::load completes.
    mLoadingResult = std::async(&Game::load, this);
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
    return false;
}
