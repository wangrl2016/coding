//
// Created by wangrl on 2021/3/8.
//

/**
 * Gif协议
 *
 * https://www.w3.org/Graphics/GIF/spec-gif89a.txt
 *
 * 参考实现1
 *
 * https://github.com/bumptech/glide/
 *
 * third_party/gif_decoder
 *
 * 参考实现2
 *
 * http://www.java2s.com/Code/Java/2D-Graphics-GUI/DecodesaGIFfileintooneormoreframes.htm
 */

#pragma once

#include <string>
#include <vector>

class GifDecoder {
public:
    GifDecoder();

    bool read(std::string path);

    int getFrameCount();

    char* getFrame(int i);

    int64_t getDelay(int i);


private:

    bool readHeader();

    bool readContent();

    std::vector<uint8_t> fileData;
};


