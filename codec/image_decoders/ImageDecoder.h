//
// Created by wangrl on 2021/3/9.
//

#pragma once

/**
 * ImageDecoder is a base for all format-specific decoders
 * (e.g. JPEGImageDecoder). This base manages the ImageFrame cache.
 */
class ImageDecoder {
public:
    enum SizeType {
        ActualSize, SizeForMemoryAllocation
    };


};
