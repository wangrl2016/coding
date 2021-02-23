//
// Created by wangrl on 2021/2/23.
//

#include <cstdlib>

extern "C" {
#include <libavformat/avformat.h>
}

int main(int argc, char** argv) {

    AVFormatContext* fmtCtx = nullptr;

    int ret;

    if (argc != 2) {
        printf("Usage: %s <input_file>\n"
               "example program to demonstrate the use of the libavformat metadata API.\n",
               argv[0]);
        return EXIT_FAILURE;
    }

    if ((ret = avformat_open_input(&fmtCtx, argv[1], nullptr, nullptr)))
        return ret;

    ret = avformat_find_stream_info(fmtCtx, nullptr);   // 查询信息流
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }

    av_dump_format(fmtCtx, 0, argv[1], 0);

    return EXIT_SUCCESS;
}
