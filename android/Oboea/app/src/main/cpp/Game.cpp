//
// Created by wangrl on 21-4-13.
//

#include "Game.h"
#include "LogMacros.h"

Game::Game(AAssetManager &assetManager) : mAssetManager(assetManager) {}

DataCallbackResult Game::onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) {
    return DataCallbackResult::Continue;
}

void Game::load() {
    if (!openStream()) {

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

    }

    return false;
}

