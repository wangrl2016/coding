//
// Created by wangrl on 2021/3/15.
//

/**
 * Simple media player based on the FFmpeg libraries.
 */

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
}

#include <SDL2/SDL.h>
#include <csignal>

#define CONFIG_AVDEVICE 1

const char programName[] = "FFplay";
const int programBirthYear = 2021;

typedef struct VideoState {

} VideoState;

// Options specified by the user

// typedef struct AVFormatContext {
// // The input container format.
// // Demuxing only, set by avformat_open_input().
// } AVFormatContext;
static AVInputFormat* fileInputFormat;  // 输入文件容器格式
static const char* inputFilename;       // 输入音视频文件
static int defaultWidth = 640;          // 默认播放窗口的宽度
static int defaultHeight = 480;         // 默认播放窗口的高度
static const char* windowTitle;

static bool audioDisable;       // 声音不播放
static bool videoDisable;       // 视频画面不播放
static bool displayDisable;     // 关闭显示窗口
static bool borderless;         // 无边框
static bool alwaysOnTop;        // 画面始终保持在窗口最顶层

// SDL相关
static SDL_Window* window;      // 播放窗口
static SDL_Renderer* renderer;  // a structure representing rendering state
static SDL_RendererInfo rendererInfo = {0};
static SDL_AudioDeviceID audioDev;

// 为WIN32平台准备
void initDynLoad() {
    // Nothing to do.
}

// 处理异常
static void sigtermHandler(int sig) {
    exit(sig);
}

// 展示程序的用法
static void showUsage(void) {
    av_log(nullptr, AV_LOG_INFO, "Simple media player\n");
    av_log(nullptr, AV_LOG_INFO, "Usage: %s [options] input_file\n", programName);
    av_log(nullptr, AV_LOG_INFO, "\n");
}

static void doExit(VideoState* is) {
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);

    exit(EXIT_SUCCESS);
}

static VideoState* streamOpen(const char* filename, AVInputFormat* inputFormat) {
    VideoState* is;
    is = static_cast<VideoState*>(av_mallocz(sizeof(VideoState)));
    if (!is)
        return nullptr;

    return is;
}

// Handle an event sent by the GUI.
static void eventLoop(VideoState* currentStream) {
    SDL_Event event;
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
    avdevice_register_all();
#endif
    avformat_network_init();

    // 初始化command参数

    signal(SIGINT, sigtermHandler);     // interrupt
    signal(SIGTERM, sigtermHandler);    // termination

    if (!inputFilename) {
        showUsage();
        av_log(nullptr, AV_LOG_FATAL, "An input file must be specified\n");
        av_log(nullptr, AV_LOG_FATAL, "Use -h to get full help\n");
        exit(EXIT_FAILURE);
    }

    if (displayDisable)
        videoDisable = true;

    flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER;
    if (audioDisable)
        flags &= ~SDL_INIT_AUDIO;
    else {
        // Try to work around an occasional ALSA buffer underflow issue when the
        // period size is NPOT due to ALSA resampling by forcing the buffer size.
        if (!SDL_getenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE"))
            SDL_setenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE", "1", 1);
    }

    if (displayDisable)
        flags &= ~SDL_INIT_VIDEO;

    if (SDL_Init(flags)) {
        av_log(nullptr, AV_LOG_FATAL, "Could not initialize SDL - %s\n", SDL_GetError());
        av_log(nullptr, AV_LOG_FATAL, "(Did you set the DISPLAY variable?)\n");
        exit(EXIT_FAILURE);
    }

    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

    // 画面显示
    if (!displayDisable) {
        int flags = SDL_WINDOW_HIDDEN;
        if (alwaysOnTop)
            flags |= SDL_WINDOW_ALWAYS_ON_TOP;
    }

    if (borderless)
        flags |= SDL_WINDOW_BORDERLESS;
    else
        flags |= SDL_WINDOW_RESIZABLE;

    window = SDL_CreateWindow(programName, SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              defaultWidth,
                              defaultHeight, flags);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    if (window) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) {
            av_log(nullptr, AV_LOG_WARNING, "Failed to initialize a hardware accelerated renderer: %s\n",
                   SDL_GetError());
            renderer = SDL_CreateRenderer(window, -1, 0);
        }
        if (renderer) {
            if (!SDL_GetRendererInfo(renderer, &rendererInfo))
                av_log(nullptr, AV_LOG_VERBOSE, "Initialized %s renderer.\n", rendererInfo.name);
        }

        if (!window || !renderer || !rendererInfo.num_texture_formats) {
            av_log(nullptr, AV_LOG_FATAL, "Failed to create window or renderer: %s", SDL_GetError());
            doExit(nullptr);
        }
    }

    is = streamOpen(inputFilename, fileInputFormat);
    if (!is) {
        av_log(nullptr, AV_LOG_FATAL, "Failed to initialize VideoState!\n");
        doExit(nullptr);
    }

    eventLoop(is);

    // Never returns.
    return EXIT_SUCCESS;
}
