//
// Created by wangrl on 2021/4/6.
//

#include "include/audio/AlResource.h"
#include "include/system/Lock.h"
#include "include/system/Mutex.h"

namespace {
    // OpenAL resources counter and its mutex
    unsigned int count = 0;
    sf::Mutex mutex;

    // The audio device is instantiated on demand rather than at global startup,
    // which solves a lot of weird crashes and errors.
    // It is destroyed when it is no longer needed.
    sf::priv::AudioDevice* globalDevice;
}

namespace sf {
    AlResource::AlResource() {
        // Protect from concurrent access.
        Lock lock(mutex);

        if (count == 0)
            globalDevice = new priv::AudioDevice;

        // Increment the resources counter.
        count++;
    }

    AlResource::~AlResource() {
        // Protected from concurrent access.
        Lock lock(mutex);

        // Decrement  the resources counter.
        count--;

        // If there's no more resource alive, we can destroy the device.
        if (count == 0)
            delete globalDevice;
    }
}