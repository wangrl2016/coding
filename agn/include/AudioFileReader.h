//
// Created by wangrl2016 on 2021/5/4.
//

#pragma once

namespace agn {
    // Performs file system I/O to decode a file (using libnyquist).

    std::shared_ptr<AudioBus> makeBusFromFile(const char* filePath, bool mixToMono);

    std::shared_ptr<AudioBus> makeBusFromFile(const std::string& path, bool mixToMono);
}