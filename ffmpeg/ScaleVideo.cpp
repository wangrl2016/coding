//
// Created by wangrl on 2021/2/26.
//

#include <cstdlib>

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/pixdesc.h>
#include <libavutil/imgutils.h>
}

#define ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))

typedef struct VideoSizeAbbr {
    const char* abbr;
    int width, height;
} VideoSizeAbbr;

static const VideoSizeAbbr VIDEO_SIZE_ABBRS[] = {
        {"vga",  640,  480},
        {"svga", 800,  600},
        {"xga",  1024, 768},
        {"uxga", 1600, 1200},
        {"qxga", 2048, 1536},
        {"sxga", 1280, 1024}
};

/**
 *
 * @param width     [out] 解析完成的宽度
 * @param height    [out] 解析完成的高度
 * @param str       需要解析的字符串
 * @return
 */
int parseVideoSize(int* width, int* height, const char* str) {
    int length = ARRAY_ELEMS(VIDEO_SIZE_ABBRS);

    int tmpWidth = 0, tmpHeight = 0;

    int i;
    for (i = 0; i < length; i++) {
        if (!strcmp(VIDEO_SIZE_ABBRS[i].abbr, str)) {
            tmpWidth = VIDEO_SIZE_ABBRS[i].width;
            tmpHeight = VIDEO_SIZE_ABBRS[i].height;
            break;
        }
    }

    char* pEnd;
    if (i == length) { // 说明没有找到缩写字符串
        tmpWidth = (int) strtol(str, &pEnd, 10);
        // 跳过宽和高分隔字符
        if (*pEnd)
            pEnd++;
        tmpHeight = (int) strtol(pEnd, &pEnd, 10);

        // Trailing extraneous data detected, like in 123x345foobar.
        if (*pEnd)
            return AVERROR(EINVAL);
    }
    if (tmpWidth <= 0 || tmpHeight <= 0)
        return AVERROR(EINVAL);

    *width = tmpWidth;
    *height = tmpHeight;

    return 0;
}

static void fillYUVImage(uint8_t* data[4], const int linesize[4],
                         int width, int height, int frameIndex) {
    int x, y;
    // Y
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++)
            data[0][y * linesize[0] + x] = x + y + frameIndex * 3;

    // Cb and Cr
    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width / 2; x++) {
            data[1][y * linesize[1] + x] = 128 + y + frameIndex * 2;
            data[2][y * linesize[2] + x] = 64 + x + frameIndex * 5;
        }
    }
}

int main(int argc, char** argv) {
    SwsContext* swsContext;
    enum AVPixelFormat srcPixFmt = AV_PIX_FMT_YUV420P, dstPixFmt = AV_PIX_FMT_RGB24;

    if (argc != 3) {
        av_log(nullptr, AV_LOG_ERROR, "Usage: %s output_file output_size\n"
                                      "API example program to show how to scale an image with libswscale.\n"
                                      "This program generate a series of pictures, rescales them to the given"
                                      "output_size and saves them to an output file name output_file\n.",
               argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* dstFilename = argv[1];
    const char* dstSize = argv[2];
    int dstWidth, dstHeight;
    int srcWidth = 320, srcHeight = 240;
    int ret;
    int dstBufferSize;
    if (parseVideoSize(&dstWidth, &dstHeight, dstSize) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Invalid size '%s', must be in the form WxH or a valid size abbreviation\n",
               dstSize);
        exit(EXIT_FAILURE);
    }

    FILE* dstFile = fopen(dstFilename, "wb");
    if (!dstFile) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open destination file %s\n", dstFilename);
        exit(EXIT_FAILURE);
    }

    swsContext = sws_getContext(srcWidth, srcHeight, srcPixFmt,
                                dstWidth, dstHeight, dstPixFmt,
                                SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!swsContext) {
        av_log(nullptr, AV_LOG_ERROR,
               "Impossible to create scale context for the conversion "
               "fmt: %s s: %dx%d -> fmt: %s s: %dx%d\n",
               av_get_pix_fmt_name(srcPixFmt), srcWidth, srcHeight,
               av_get_pix_fmt_name(dstPixFmt), dstWidth, dstHeight);
        ret = AVERROR(EINVAL);
        goto end;
    }

    uint8_t* srcData[4], * dstData[4];
    int srcLinesize[4], dstLinesize[4];

    // Allocate source and destination image buffers
    if ((ret = av_image_alloc(srcData, srcLinesize,
                              srcWidth, srcHeight, srcPixFmt, 16)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate source image\n");
        goto end;
    }

    // Buffer is going to be written to rawvideo file, no alignment.
    if ((ret = av_image_alloc(dstData, dstLinesize,
                              dstWidth, dstHeight, dstPixFmt, 1)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate destination image\n");
        goto end;
    }

    dstBufferSize = ret;

    for (int i = 0; i < 100; i++) {
        // Generate synthetic video.
        fillYUVImage(srcData, srcLinesize, srcWidth, srcHeight, i);

        // Convert to destination format.
        sws_scale(swsContext, (const uint8_t* const*) srcData,
                  srcLinesize, 0, srcHeight, dstData, dstLinesize);

        // Write scaled image to file
        fwrite(dstData[0], 1, dstBufferSize, dstFile);
    }

    av_log(nullptr, AV_LOG_INFO, "Scaling succeed. Play the output file with the command:\n"
                                 "ffplay -f rawvideo -pix_fmt %s -video_size %dx%d %s\n",
           av_get_pix_fmt_name(dstPixFmt), dstWidth, dstHeight, dstFilename);

    end:
    fclose(dstFile);
    av_freep(&srcData[0]);
    av_freep(&dstData[0]);
    sws_freeContext(swsContext);
    return ret < 0;
}
