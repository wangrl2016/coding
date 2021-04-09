//
// Created by wangrl on 21-4-9.
//

#pragma once

#include "LogMacros.h"
#include "oboe/AudioStreamCallback.h"

/**
 * This is a callback object which will be called when a stream error occurs.
 *
 * It is constructed using a IRestartable which allows it to automatically restart the parent
 * object if the stream is disconnected (for example, when headphones are attached).
 *
 * @param IRestartable - the object which should be restarted when the stream is disconnected
 */
class DefaultErrorCallback : public oboe::AudioStreamErrorCallback {
public:
    DefaultErrorCallback(IRestartable &parent) : mParent(parent) {}

    virtual ~DefaultErrorCallback() = default;

    void onErrorAfterClose(oboe::AudioStream * oboeStream, oboe::Result error) override {
        // Restart the stream if the error is a disconnect, otherwise do nothing and log the error
        // reason.
        if (error == oboe::Result::ErrorDisconnected) {
            LOGI("Restarting AudioStream");
            mParent.restart();
        }
        LOGE("Error was %d", oboe::convertToText(error));
    }

private:
    IRestartable &mParent;
};