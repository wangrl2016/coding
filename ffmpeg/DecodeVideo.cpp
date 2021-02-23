//
// Created by wangrl on 2021/2/23.
//

// https://github.com/leandromoreira/ffmpeg-libav-tutorial

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}

int main(int argc, char* argv[]) {

    // AVFormatContext holds the header information from the format (Container).
    // Allocating memory for this component.
    AVFormatContext* fmtCtx = avformat_alloc_context();
    if (!fmtCtx) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate memory for Format Context\n");
        return EXIT_FAILURE;
    }

    // Open the file and read its header. The codecs are not opened.
    // The function arguments are:
    //
}
