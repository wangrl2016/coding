//
// Created by wangrl on 2021/3/15.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

#include <SDL2/SDL.h>

typedef struct VideoState {

} VideoState;

// 为WIN32平台准备
void initDynLoad() {
    // Nothing to do.
}

// Called from the main
int main(int argc, char** argv) {
    int flags;

    // 保存和视频相关的所有信息
    VideoState* is;

    initDynLoad();

    av_log_set_flags(AV_LOG_SKIP_REPEATED);

    // Register all codec, demux and protocols.
#if CONFIG_AVDEVICE
    printf("avdevice\n");
#endif


    return EXIT_SUCCESS;
}