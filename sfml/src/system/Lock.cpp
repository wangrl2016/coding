//
// Created by wangrl on 2021/4/7.
//

#include <include/system/Lock.h>

namespace sf {
    Lock::Lock(Mutex& mutex) : mMutex(mutex) {
        mMutex.lock();
    }

    Lock::~Lock() {
        mMutex.unlock();
    }

}



