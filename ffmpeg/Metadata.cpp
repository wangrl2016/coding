//
// Created by wangrl on 2021/2/23.
//

extern "C" {
#include <libavformat/avformat.h>
}

int main(int argc, char** argv) {

    AVFormatContext* fmtCtx = nullptr;
    AVDictionaryEntry* tag = nullptr;

    int ret;

    if (argc != 2) {
        printf("Usage: %s <input_file>\n"
               "example program to demonstrate the use of the libavformat metadata API.\n",
               argv[0]);
        return EXIT_FAILURE;
    }

    if (ret = avformat_open_input(&fmtCtx, argv[1], nullptr, nullptr))
        return ret;

    if ((ret = avformat_find_stream_info(fmtCtx, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }

    while ((tag = av_dict_get(fmtCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX)))
        printf("%s = %s\n", tag->key, tag->value);

    avformat_close_input(&fmtCtx);

    return EXIT_SUCCESS;
}