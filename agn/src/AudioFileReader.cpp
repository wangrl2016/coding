//
// Created by wangrl2016 on 2021/5/4.
//

#include <memory>
#include <mutex>

#include "AudioBus.h"
#include "AudioFileReader.h"

namespace agn {

    std::mutex gFileIOMutex;
    std::shared_ptr<AudioBus> makeBusFromFile(const char* filePath, bool mixToMono) {

    }
}
