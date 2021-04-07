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


}

namespace sf {
    AlResource::AlResource() {
        // Protect from concurrent access
        Lock lock(mutex);
    }
}