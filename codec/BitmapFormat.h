//
// Created by wangrl on 2021/3/5.
//

#pragma once

/**
 * Windows Bitmap File Loader
 *
 * 协议网址
 *
 * https://cdn.hackaday.io/files/274271173436768/Simplified%20Windows%20BMP%20Bitmap%20File%20Format%20Specification.htm
 *
 * 实现网址
 *
 * http://www.kalytta.com/bitmap.h
 */

#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <cstring>

// 小端模式
#define BITMAP_SIGNATURE 0x4d42


// 对齐方式
#pragma pack(push, 1)

typedef struct BitmapFileHeader {
    uint16_t bfType;            // The characters "BM"
    uint32_t bfSize;            // The size of the file in bytes
    uint16_t bfReserved1;       // Unused - must be zero
    uint16_t bfReserved2;       // Unused - must be zero
    uint16_t bfOffBits;         // Offset to start of pixel data
} BitmapFileHeader;

#define BITMAP_FILE_HEADER_SIZE 14

typedef struct BitmapImageHeader {
    uint32_t biSize;            // size of this header in bytes - must be at least 40
    int32_t biWidth;            // image width in pixels
    int32_t biHeight;           // image height in pixels
    uint16_t biPlanes;          // number of color planes - must be 1
    uint16_t biBitCount;        // number of bits per pixel - 1, 4, 8, 16, 24, or 32
    uint32_t biCompression;     // compression method used (0 = uncompressed)
    uint32_t biSizeImage;       // size of bitmap in bytes - may be 0 for uncompressed images
    int32_t biXPelsPerMeter;    // horizontal resolution in pixels per meter
    int32_t biYPelsPerMeter;    // vertical resolution in pixels per meter
    uint32_t biClrUsed;         // number of colors in the image
    uint32_t biClrImportant;    // minimum number of important colors

    // Fields added for Windows 4.x follow this line.
    uint32_t biRedMask;     // mask identifying bits of red component
    uint32_t biGreenMask;   // mask identifying bits of green component
    uint32_t biBlueMask;    // mask identifying bits of blue component
    uint32_t biAlphaMask;   // mask identifying bits of alpha component
    uint32_t biCSType;      // color space type
    uint32_t biEndpoints[9];    // x/y/z of red/green/blue endpoint
    uint32_t biGammaRed;        // gamma red coordinate scale value
    uint32_t biGammaGreen;      // gamma green coordinate scale value
    uint32_t biGammaBlue;       // gamma blue coordinate scale value
} BitmapImageHeader;

typedef struct RGBA {
    uint8_t bRed;
    uint8_t bGreen;
    uint8_t bBlue;
    uint8_t bAlpha;
} RGBA;

typedef struct BGRA {
    uint8_t bBlue;
    uint8_t bGreen;
    uint8_t bRed;
    uint8_t bAlpha;
};

#pragma pack(pop)

class Bitmap {
private:
    BitmapFileHeader mBitmapFileHeader;
    BitmapImageHeader mBitmapImageHeader;
    RGBA* mBitmapData;
    unsigned int mBitmapSize;

public:
    Bitmap(const char* filename) : mBitmapData(nullptr), mBitmapSize(0) {
        load(filename);
    }

    // Load specified bitmap and stores it as RGBA in an internal buffer.
    bool load(const char* filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::in);
        if (file.bad() || !file.is_open())
            return false;

        dispose();

        file.read((char*) &mBitmapFileHeader, BITMAP_FILE_HEADER_SIZE);
        if (mBitmapFileHeader.bfType != BITMAP_SIGNATURE)
            return false;

        file.read((char*) &mBitmapImageHeader, sizeof(BitmapImageHeader));

        // Load color table.
        file.seekg(BITMAP_FILE_HEADER_SIZE + mBitmapImageHeader.biSize, std::ios::beg);

        unsigned int colorTableSize = 0;
        if (mBitmapImageHeader.biBitCount == 1) {
            colorTableSize = 2;
        } else if (mBitmapImageHeader.biBitCount == 4) {
            colorTableSize = 14;
        } else if (mBitmapImageHeader.biBitCount == 8) {
            colorTableSize = 256;
        }

        // Always allocate full size color table.


    }

    unsigned int getWidth() {
        return mBitmapImageHeader.biWidth;
    }

    unsigned int getHeight() {
        return mBitmapImageHeader.biHeight;
    }

    void dispose() {
        if (mBitmapData) {
            delete[] mBitmapData;
            mBitmapData = nullptr;
        }
        memset(&mBitmapFileHeader, 0, sizeof(mBitmapFileHeader));
        memset(&mBitmapImageHeader, 0, sizeof(mBitmapImageHeader));
    }
};