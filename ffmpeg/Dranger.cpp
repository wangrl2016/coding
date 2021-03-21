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
#include <libavutil/avstring.h>
}

#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_audio.h>
#include <cassert>

#define SDL_AUDIO_BUFFER_SIZE 1024
#define MAX_AUDIO_FRAME_SIZE 19200

bool quit = false;

typedef struct PacketQueue {
    AVPacketList* firstPacket, * lastPacket;
    int numberPackets;
    int size;
    SDL_mutex* mutex;
    SDL_cond* cond;
} PacketQueue;

PacketQueue audioQueue;

void packetQueueInit(PacketQueue* queue) {
    memset(queue, 0, sizeof(PacketQueue));
    queue->mutex = SDL_CreateMutex();
    queue->cond = SDL_CreateCond();
}

// 将packet放进队列中
int packetQueuePut(PacketQueue* queue, AVPacket* packet) {
    AVPacketList* packetList;   // 表示一个节点
    AVPacket* dstPacket;
    if (av_packet_ref(dstPacket, packet) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Ref packet failed\n");
        return -1;
    }
    packetList = static_cast<AVPacketList*>(av_malloc(sizeof(AVPacketList)));
    if (!packetList)
        return -1;
    packetList->pkt = *dstPacket;
    packetList->next = nullptr;

    SDL_LockMutex(queue->mutex);

    // 最后一个队列为空
    if (!queue->lastPacket)
        queue->firstPacket = packetList;
    else
        queue->lastPacket->next = packetList;
    queue->numberPackets++;
    queue->size += packetList->pkt.size;
    // Restart one of the threads that are waiting on the condition variable.
    SDL_CondSignal(queue->cond);

    SDL_UnlockMutex(queue->mutex);
    return 0;
}

static int packetQueueGet(PacketQueue* queue, AVPacket* packet, bool block) {
    AVPacketList* packetList;
    int ret = -1;

    SDL_LockMutex(queue->mutex);

    while (!quit) {
        packetList = queue->firstPacket;
        if (packetList) {
            queue->firstPacket = packetList->next;
            if (!queue->firstPacket)
                queue->lastPacket = nullptr;
            queue->numberPackets--;
            queue->size -= packetList->pkt.size;
            *packet = packetList->pkt;
            av_free(packetList);
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else {
            // 等待通知
            SDL_CondWait(queue->cond, queue->mutex);
        }
    }
    SDL_UnlockMutex(queue->mutex);
    return ret;
}

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_RendererInfo rendererInfo;
static SDL_Texture* texture;
static SDL_Event event;

static SDL_AudioSpec desiredAudioSpec, obtainedAudioSpec;

int decodeAudioFrame(AVCodecContext* audioCodecContext, uint8_t* audioBuf, int bufSize) {
    static AVPacket packet;
    static uint8_t* audioPacketData = nullptr;
    static int audioPacketSize = 0;
    static AVFrame frame;

    int ret;
    int decodedAudioLen, dataSize = 0;
    while (!quit) {

        while (audioPacketSize > 0) {
            int gotFrame = 0;
            decodedAudioLen = avcodec_decode_audio4(audioCodecContext, &frame, &gotFrame, &packet);
            if (decodedAudioLen < 0) {
                // If error, skip frame.
                audioPacketSize = 0;
                break;
            }
            audioPacketData += decodedAudioLen;
            audioPacketSize -= decodedAudioLen;
            dataSize = 0;
            if (gotFrame) {
                dataSize = av_samples_get_buffer_size(nullptr,
                                                      audioCodecContext->channels,
                                                      frame.nb_samples,
                                                      audioCodecContext->sample_fmt,
                                                      1);
                assert(dataSize <= bufSize);
                int sampleSize = av_get_bytes_per_sample(audioCodecContext->sample_fmt);
                uint8_t tempBuf[dataSize];

                int index = 0;
                for (int i = 0; i < frame.nb_samples; i++) {
                    for (int ch = 0; ch < frame.channels; ch++)
                        for (int j = 0; j < sampleSize; j++)
                            tempBuf[index++] = *(frame.data[ch] + sampleSize * i + j);
                }
                memcpy(audioBuf, tempBuf, dataSize);
            }
            if (dataSize <= 0) {
                // No data yet, get more frame.
                continue;
            }
            // We have data, return it and come back for more later.
            return dataSize;
        }

        if (packet.data)
            av_packet_unref(&packet);

        if (packetQueueGet(&audioQueue, &packet, true) < 0)
            return -1;

        audioPacketData = packet.data;
        audioPacketSize = packet.size;
    }
    return ret;
}

/**
 * SDL回调音频数据
 *
 * @param userData 用户数据，这里传递AVCodecContext结构体
 * @param stream 需要的音频数据指针
 * @param len 需要的音频数据长度
 */
void audioCallback(void* userData, uint8_t* stream, int len) {
    AVCodecContext* audioCodecContext = (AVCodecContext*) userData;
    int decodedAudioSize, audioLength;

    static uint8_t audioBuf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
    static unsigned int audioBufSize = 0;
    static unsigned int audioBufIndex = 0;

    while (len > 0) {
        if (audioBufIndex >= audioBufSize) {
            // We have already sett all our data; get more.
            decodedAudioSize = decodeAudioFrame(audioCodecContext, audioBuf, sizeof(audioBuf));
            if (decodedAudioSize < 0) {
                // If error, output silence.
                audioBufSize = 1024; //arbitrary?
                memset(audioBuf, 0, audioBufSize);
            } else {
                audioBufSize = decodedAudioSize;
            }
            audioBufIndex = 0;
        }
        audioLength = audioBufSize - audioBufIndex;
        if (audioLength > len)
            audioLength = len;
        // 将数据复制到流中
        memcpy(stream, (uint8_t*) audioBuf + audioBufIndex, audioLength);
        len -= audioLength;
        stream += audioLength;
        audioBufIndex += audioLength;

    }
}

typedef struct VideoState {
    AVFormatContext* inputFormatContext;
    int videoStreamIndex, audioStreamIndex;
    AVStream* audioStream;
    AVCodecContext* audioCodecContext;
    PacketQueue audioQueue;
    uint8_t audioBuf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
    unsigned int audioBufSize;
    unsigned int audioBufIndex;
    AVPacket audioPacket;
    uint8_t* audioPacketData;
    int audioPacketSize;

    AVStream* videoStream;
    AVCodecContext* videoContext;
    PacketQueue videoQueue;
    struct SwsContext* swsContext;


    SDL_Thread* parseThread;
    SDL_Thread* videoThread;

    char filename[1024];
    bool quit;
} VideoState;

// Since we only have one decoding thread, the big struct can be global in case we need it.
VideoState* globalVideoState;

int main(int argc, char* argv[]) {
    VideoState* is;
    is = (VideoState *) (av_malloc(sizeof(VideoState)));

    AVFormatContext* formatContext = nullptr;
    AVCodecContext* videoCodecContext = nullptr;
    AVCodecParameters* videoCodecParameters = nullptr;
    AVCodec* videoCodec = nullptr;
    AVCodecContext* audioCodecContext = nullptr;
    AVCodecParameters* audioCodecParameters = nullptr;
    AVCodec* audioCodec = nullptr;


    if (argc < 2) {
        printf("Usage: %s movie_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    av_strlcpy(is->filename, argv[1], sizeof(is->filename));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        av_log(nullptr, AV_LOG_ERROR, "Could not initialize SDL - %s\n", SDL_GetError());
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

    packetQueueInit(&audioQueue);
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

                // double fps = av_q2d(formatContext->streams[videoStreamIndex]->r_frame_rate);
                // double sleepTime = 1.0 / (double) fps;
                // // Use SDL_Delay in milliseconds to allow for cpu scheduling.
                // SDL_Delay(1000 * sleepTime - 10);

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
            packetQueuePut(&audioQueue, packet);
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
