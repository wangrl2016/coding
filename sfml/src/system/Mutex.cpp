//
// Created by wangrl on 2021/4/7.
//

#include "include/system/Mutex.h"
#include "include/system/MutexImpl.h"

namespace sf {
    namespace priv {

    }

    Mutex::Mutex() {
        mMutexImpl = new priv::MutexImpl;
    }

    Mutex::~Mutex() {
        delete mMutexImpl;
    }

    void Mutex::lock() {
        mMutexImpl->lock();
    }

    void Mutex::unlock() {
        mMutexImpl->unlock();
    }
}
