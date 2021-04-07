//
// Created by wangrl on 2021/4/7.
//

#include <include/system/Mutex.h>

namespace sf {
    namespace priv {

    }

    Mutex::Mutex() {
        mMutexImpl = new priv::MutexImpl;
    }
}
