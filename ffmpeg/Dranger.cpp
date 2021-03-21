//
// Created by wangrl on 2021/3/20.
//

/**
 * 使用SDL的播放器
 *
 * 参考链接
 *
 * http://www.dranger.com/ffmpeg
 *
 * 播放音频的原理
 *
 * Digital audio consists of a long stream of samples. Each sample represents a value
 * of the audio waveform. Sound are recorded at a certain sample rate, which
 * simple says how fast to play each sample, and is measured in number of samples per
 * second.
 *
 * 常见的采样频率有22050和44100
 *
 * In addition, most audio can have more than one channel for stereo or surround, so for
 * example, if the sample is in stereo, the samples will come 2 at a time.
 *
 * SDL播放音频原理
 * 1. 设置音频参数，比如采样率，声道数等。
 * 2. 设置回调函数，SDL会调用回调函数填充数据。
 */

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}

#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_audio.h>

#define SDL_AUDIO_BUFFER_SIZE 1024

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_RendererInfo rendererInfo;
static SDL_Texture* texture;
static SDL_Event event;

static SDL_AudioSpec desiredAudioSpec, obtainedAudioSpec;

void audioCallback(void* userData, uint8_t* stream, int len) {

}

int main(int argc, char* argv[]) {
    AVFormatContext* formatContext = nullptr;
    AVCodecContext* videoCodecContext = nullptr;
    AVCodecParameters* videoCodecParameters = nullptr;
    AVCodec* videoCodec = nullptr;
    AVCodecContext* audioCodecContext = nullptr;
    AVCodecParameters* audioCodecParameters = nullptr;
    AVCodec* audioCodec = nullptr;
    bool quit = false;

    if (argc < 2) {
        printf("Usage: %s movie_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        av_log(nullptr, AV_LOG_ERROR, "Could not initialize SDL - %d\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // open video file
    if (avformat_open_input(&formatContext, argv[1], nullptr, nullptr) != 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open video file\n");
        return EXIT_FAILURE;
    }

    // retrieve stream information
    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find stream info\n");
        return EXIT_FAILURE;
    }

    // Dump information about file onto standard output.
    av_dump_format(formatContext, 0, argv[1], 0);

    // find the first video stream
    int videoStreamIndex = -1;
    int audioStreamIndex = -1;
    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO
            && videoStreamIndex < 0) {
            videoStreamIndex = i;
        }
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO
            && audioStreamIndex < 0) {
            audioStreamIndex = i;
        }
    }

    if (videoStreamIndex == -1 || audioStreamIndex == -1) { return -1; }

    videoCodecParameters = formatContext->streams[videoStreamIndex]->codecpar;
    videoCodec = avcodec_find_decoder(videoCodecParameters->codec_id);
    if (!videoCodec) {
        fprintf(stderr, "unsupported videoCodec\n");
        return -1;
    }

    videoCodecContext = avcodec_alloc_context3(videoCodec);
    if (avcodec_parameters_to_context(videoCodecContext, videoCodecParameters)) {
        fprintf(stderr, "could not copy videoCodec context\n");
        return -1;
    }

    // initialize the AVCodecContext to use the given AVCodec
    if (avcodec_open2(videoCodecContext, videoCodec, nullptr) < 0) {
        return -1;
    }

    audioCodecParameters = formatContext->streams[audioStreamIndex]->codecpar;
    audioCodec = avcodec_find_decoder(audioCodecParameters->codec_id);
    if (!audioCodec) {
        fprintf(stderr, "Unsupported audioCodec\n");
        return -1;
    }

    audioCodecContext = avcodec_alloc_context3(audioCodec);
    if (avcodec_parameters_to_context(audioCodecContext, audioCodecParameters)) {
        fprintf(stderr, "could not copy audioCodec context\n");
        return -1;
    }

    if (avcodec_open2(audioCodecContext, audioCodec, nullptr) < 0) {
        return -1;
    }

    AVFrame* frame = av_frame_alloc();
    AVPacket* packet = av_packet_alloc();

    window = SDL_CreateWindow("Dranger",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              videoCodecContext->width,
                              videoCodecContext->height,
                              SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    if (window) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer) {
            av_log(nullptr, AV_LOG_WARNING, "Failed to initialize a hardware accelerated renderer: %s\n",
                   SDL_GetError());
            renderer = SDL_CreateRenderer(window, -1, 0);
        }
        if (renderer) {
            if (!SDL_GetRendererInfo(renderer, &rendererInfo)) {
                av_log(nullptr, AV_LOG_VERBOSE, "Initialized %s renderer\n", rendererInfo.name);
            }
        }
    }
    if (!window || !renderer || !rendererInfo.num_texture_formats) {
        av_log(nullptr, AV_LOG_FATAL, "Failed to create window or renderer: %s",
               SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Use this function to create a texture for a rendering context.
    // YUV420P that is the same as YV12.
    texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_YV12,
            SDL_TEXTUREACCESS_STREAMING,
            videoCodecContext->width,
            videoCodecContext->height);

    // Set audio settings from codec info
    SDL_memset(&desiredAudioSpec, 0, sizeof(desiredAudioSpec));
    desiredAudioSpec.freq = audioCodecContext->sample_rate;
    desiredAudioSpec.format = AUDIO_F32LSB;
    desiredAudioSpec.channels = audioCodecContext->channels;
    desiredAudioSpec.silence = 0;
    desiredAudioSpec.samples = SDL_AUDIO_BUFFER_SIZE;
    desiredAudioSpec.callback = audioCallback;
    desiredAudioSpec.userdata = audioCodecContext;
    if (SDL_OpenAudio(&desiredAudioSpec, &obtainedAudioSpec) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "SDL_OpenAudio: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    } else {
        // format 0x8120 => 33056
        av_log(nullptr, AV_LOG_INFO, "Audio params - sample_rate %d, format %d, channels %d, ",
               obtainedAudioSpec.freq, obtainedAudioSpec.format, obtainedAudioSpec.channels);
    }

    // Start audio playing.
    SDL_PauseAudio(0);

    while (av_read_frame(formatContext, packet) >= 0 && !quit) {
        // Is this a packet from the video stream?
        if (packet->stream_index == videoStreamIndex) {
            // supply raw packet data as input to a decoder
            int response = avcodec_send_packet(videoCodecContext, packet);
            if (response < 0) {
                av_log(nullptr, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
                return response;
            }
            while (response >= 0) {
                // return decode output data (into a frame) from a decoder
                response = avcodec_receive_frame(videoCodecContext, frame);
                if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
                    break;
                } else if (response < 0) {
                    av_log(nullptr, AV_LOG_ERROR, "Error while receiving a frame from the decoder\n");
                    return response;
                }

                double fps = av_q2d(formatContext->streams[videoStreamIndex]->r_frame_rate);
                double sleepTime = 1.0 / (double) fps;
                // Use SDL_Delay in milliseconds to allow for cpu scheduling.
                SDL_Delay(1000 * sleepTime - 10);

                // The simplest struct in SDL. It contains only four shorts.
                // x, y which holds the position and w, h which holds width and height.
                // It's important to note that 0, 0 is the upper left corner in SDL.
                // So the higher y value means lower, and the bottom right corner will
                // have the coordinate x + w, y + h.
                SDL_Rect rect;
                rect.x = 0;
                rect.y = 0;
                rect.w = videoCodecContext->width;
                rect.h = videoCodecContext->height;

                // Update a rectangle within a planar YV12 texture with new pixel data.
                SDL_UpdateYUVTexture(
                        texture,
                        &rect,
                        frame->data[0],
                        frame->linesize[0],
                        frame->data[1],
                        frame->linesize[1],
                        frame->data[2],
                        frame->linesize[2]);

                // Clear the current rendering target with the drawing color.
                SDL_RenderClear(renderer);

                // Copy a portion of the texture to the current rendering target.
                SDL_RenderCopy(renderer, texture, nullptr, nullptr);

                // Update the screen with any rendering performed since the previous call.
                SDL_RenderPresent(renderer);
            }
        } else if (packet->stream_index == audioStreamIndex) {

        }
        // free the packet that was allocated by av_read_frame
        av_packet_unref(packet);

        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT: {
                quit = true;
                break;
            }
        }
    }

    av_packet_free(&packet);
    av_frame_free(&frame);

    avcodec_free_context(&videoCodecContext);
    avcodec_free_context(&audioCodecContext);
    avformat_close_input(&formatContext);
    return 0;
}
