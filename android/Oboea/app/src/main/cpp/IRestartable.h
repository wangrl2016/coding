//
// Created by wangrl on 21-4-9.
//

#pragma once

/**
 * Represents an object which can be restarted. For example an audio engine which has one or more
 * streams which can be restarted following a change in audio device configuration. For example,
 * headphones being connected.
 */
class IRestartable {
public:
    virtual void restart() = 0;
};
