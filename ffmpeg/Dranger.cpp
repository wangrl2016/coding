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
 *
 */

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}

#include <SDL.h>
#include <SDL_thread.h>

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_RendererInfo rendererInfo;
static SDL_Texture* texture;

int main(int argc, char* argv[]) {
    AVFormatContext* formatContext = nullptr;
    AVCodecContext* codecContext = nullptr;
    AVCodecParameters* codecParameters = nullptr;
    AVCodec* codec = nullptr;

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
    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }
    if (videoStreamIndex == -1) { return -1; }

    codecParameters = formatContext->streams[videoStreamIndex]->codecpar;
    codec = avcodec_find_decoder(codecParameters->codec_id);
    if (!codec) {
        fprintf(stderr, "unsupported codec\n");
        return -1;
    }

    codecContext = avcodec_alloc_context3(codec);
    if (avcodec_parameters_to_context(codecContext, codecParameters)) {
        fprintf(stderr, "could not copy codec context\n");
        return -1;
    }

    // initialize the AVCodecContext to use the given AVCodec
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        return -1;
    }

    AVFrame* frame = av_frame_alloc();
    AVPacket* packet = av_packet_alloc();

    // initialize SwsContext for scaling
    SwsContext* swsContext = sws_getContext(
            codecContext->width,
            codecContext->height,
            codecContext->pix_fmt,
            codecContext->width,
            codecContext->height,
            AV_PIX_FMT_RGB24,
            SWS_BILINEAR,
            nullptr, nullptr, nullptr);

    window = SDL_CreateWindow("Dranger",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              codecContext->width,
                              codecContext->height,
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
            codecContext->width,
            codecContext->height);

    while (av_read_frame(formatContext, packet) >= 0) {
        // Is this a packet from the video stream?
        if (packet->stream_index == videoStreamIndex) {
            // supply raw packet data as input to a decoder
            int response = avcodec_send_packet(codecContext, packet);
            if (response < 0) {
                av_log(nullptr, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
                return response;
            }
            while (response >= 0) {
                // return decode output data (into a frame) from a decoder
                response = avcodec_receive_frame(codecContext, frame);
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
                rect.w = codecContext->width;
                rect.h = codecContext->height;

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
        }
        // free the packet that was allocated by av_read_frame
        av_packet_unref(packet);
    }

    av_packet_free(&packet);
    av_frame_free(&frame);

    avcodec_free_context(&codecContext);
    sws_freeContext(swsContext);
    avformat_close_input(&formatContext);
    return 0;
}
