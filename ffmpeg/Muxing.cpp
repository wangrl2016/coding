//
// Created by wangrl on 2021/3/4.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/timestamp.h>
#include <libswscale/swscale.h>
#include <libavutil/avassert.h>
}

#define STREAM_FRAME_RATE 24
#define STREAM_DURATION 10
#define CODEC_CAP_VARIABLE_FRAME_SIZE (1u << 16u)
#define CH_FRONT_LEFT             0x00000001u
#define CH_FRONT_RIGHT            0x00000002u
#define CH_LAYOUT_STEREO          (CH_FRONT_LEFT|CH_FRONT_RIGHT)

// A wrapper around a single output AVStream.
typedef struct OutputStream {
    AVStream* stream;
    AVCodecContext* codecContext;

    // Pts of the next frame that will be generated.
    int64_t nextPts;

    int samplesCount;

    AVFrame* frame;
    AVFrame* tmpFrame;

    // 声音生成相关参数
    float t, tincr, tincr2;

    struct SwsContext* swsContext;
    struct SwrContext* swrContext;
} OutputStream;

// Add an output stream.
static void addStream(OutputStream* outputStream, AVFormatContext* formatContext,
                      AVCodec** codec, enum AVCodecID codecId) {
    AVCodecContext* codecContext;
    // find the encoder
    *codec = avcodec_find_encoder(codecId);
    if (!(*codec)) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find encoder for '%s'\n",
               avcodec_get_name(codecId));
        exit(EXIT_FAILURE);
    }

    outputStream->stream = avformat_new_stream(formatContext, nullptr);
    if (!outputStream->stream) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate stream\n");
        exit(EXIT_FAILURE);
    }
    outputStream->stream->id = ((int) formatContext->nb_streams) - 1;
    codecContext = avcodec_alloc_context3(*codec);
    if (!codecContext) {
        av_log(nullptr, AV_LOG_ERROR, "Could not alloc an encoding context\n");
        exit(EXIT_FAILURE);
    }
    outputStream->codecContext = codecContext;
    switch ((*codec)->type) {
        case AVMEDIA_TYPE_AUDIO: {
            codecContext->sample_fmt = (*codec)->sample_fmts ?
                                       (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
            codecContext->bit_rate = 64000;
            codecContext->sample_rate = 44100;
            if ((*codec)->supported_samplerates) {
                codecContext->sample_rate = (*codec)->supported_samplerates[0];
                for (int c = 0; (*codec)->supported_samplerates[c]; c++) {
                    if ((*codec)->supported_samplerates[c] == 44100)
                        codecContext->sample_rate = 44100;
                }
            }
            codecContext->channels = av_get_channel_layout_nb_channels(codecContext->channel_layout);
            codecContext->channel_layout = CH_LAYOUT_STEREO;
            if ((*codec)->channel_layouts) {
                codecContext->channel_layout = (*codec)->channel_layouts[0];
                for (int i = 0; (*codec)->channel_layouts[i]; i++) {
                    if ((*codec)->channel_layouts[i] == CH_LAYOUT_STEREO)
                        codecContext->channel_layout = CH_LAYOUT_STEREO;
                }
            }
            codecContext->channels = av_get_channel_layout_nb_channels(codecContext->channel_layout);
            outputStream->stream->time_base = (AVRational) {1, codecContext->sample_rate};
            break;
        }
        case AVMEDIA_TYPE_VIDEO: {
            codecContext->codec_id = codecId;
            codecContext->bit_rate = 400000;
            // Resolution must be a multiple of two.
            codecContext->width = 352;
            codecContext->height = 288;

            // Timebase: this is the fundamental unit of time (in seconds) in terms
            // of which frame timestamps are represented. For fixed-fps content,
            // timebase should be 1/framerate and timestamp increments should be
            // identical to 1.
            outputStream->stream->time_base = (AVRational) {1, STREAM_FRAME_RATE};
            codecContext->time_base = outputStream->stream->time_base;

            // Emit one intra frame every twelve frame at most.
            codecContext->gop_size = 12;
            codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
            if (codecContext->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
                // Just for testing, we also add b-frames.
                codecContext->max_b_frames = 2;
            }
            if (codecContext->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
                // Needed to avoid using macro blocks in which some coeffs overflow.
                // This does not happen with normal video, it just happens here as
                // the motion of the chroma plane does not match the luma plane.
                codecContext->mb_decision = 2;
            }
            break;
        }
        default:
            break;
    }
}

static AVFrame* allocPicture(enum AVPixelFormat pixelFormat, int width, int height) {
    AVFrame* frame;
    int ret;
    frame = av_frame_alloc();
    if (!frame)
        return nullptr;

    frame->format = pixelFormat;
    frame->width = width;
    frame->height = height;

    // Allocate the buffers for the frame data.
    ret = av_frame_get_buffer(frame, 0);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate frame data\n");
        exit(EXIT_FAILURE);
    }
    return frame;
}

static void openVideo(AVCodec* codec, OutputStream* outputStream, AVDictionary* option) {
    int ret;
    AVCodecContext* codecContext = outputStream->codecContext;
    AVDictionary* opt = nullptr;
    av_dict_copy(&opt, option, 0);

    // Open the codec.
    ret = avcodec_open2(codecContext, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open video codec\n");
        exit(EXIT_FAILURE);
    }

    // Allocate and init a re-usable frame.
    outputStream->frame = allocPicture(codecContext->pix_fmt, codecContext->width, codecContext->height);
    if (!outputStream->frame) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate video frame\n");
        exit(EXIT_FAILURE);
    }

    // If the output format is not YUV420P, then a temporary YUV420P
    // picture is needed too. It is then converted to the required
    // output format.
    outputStream->tmpFrame = nullptr;
    if (codecContext->pix_fmt != AV_PIX_FMT_YUV420P) {
        outputStream->tmpFrame = allocPicture(AV_PIX_FMT_YUV420P, codecContext->width, codecContext->height);
        if (!outputStream->tmpFrame) {
            av_log(nullptr, AV_LOG_ERROR, "Could not allocate temporary picture\n");
            exit(EXIT_FAILURE);
        }
    }
    // Copy the stream parameters to the muxer.
    ret = avcodec_parameters_from_context(outputStream->stream->codecpar, codecContext);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not copy the stream parameters\n");
        exit(EXIT_FAILURE);
    }
}

static AVFrame* allocAudioFrame(enum AVSampleFormat sampleFormat,
                                uint64_t channelLayout, int sampleRate, int nbSamples) {
    AVFrame* frame = av_frame_alloc();
    int ret;
    if (!frame) {
        av_log(nullptr, AV_LOG_ERROR, "Error allocating an audio frame\n");
        exit(EXIT_FAILURE);
    }

    frame->format = sampleFormat;
    frame->channel_layout = channelLayout;
    frame->sample_rate = sampleRate;
    frame->nb_samples = nbSamples;

    if (nbSamples) {
        ret = av_frame_get_buffer(frame, 0);
        if (ret < 0) {
            fprintf(stderr, "Error allocating an audio buffer\n");
            exit(EXIT_FAILURE);
        }
    }
    return frame;
}

static void openAudio(AVCodec* codec, OutputStream* outputStream, AVDictionary* option) {
    AVCodecContext* codecContext;
    int nbSamples;
    int ret;
    AVDictionary* opt = nullptr;
    codecContext = outputStream->codecContext;

    // Open it.
    av_dict_copy(&opt, option, 0);
    ret = avcodec_open2(codecContext, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open audio codec\n");
        exit(EXIT_FAILURE);
    }

    // Init signal generator.
    outputStream->t = 0;
    outputStream->tincr = 2.0f * ((float) M_PI) * 110.0f / (float) codecContext->sample_rate;
    outputStream->tincr2 =
            2.0f * ((float) M_PI) * 110.0f / (float) codecContext->sample_rate / (float) codecContext->sample_rate;

    if (((unsigned) codecContext->codec->capabilities) & ((unsigned) CODEC_CAP_VARIABLE_FRAME_SIZE))
        nbSamples = 10000;
    else
        nbSamples = codecContext->frame_size;

    outputStream->frame = allocAudioFrame(codecContext->sample_fmt, codecContext->channel_layout,
                                          codecContext->sample_rate, nbSamples);

    outputStream->tmpFrame = allocAudioFrame(AV_SAMPLE_FMT_S16, codecContext->channel_layout,
                                             codecContext->sample_rate, nbSamples);

    // Copy the stream parameters to the muxer.
    ret = avcodec_parameters_from_context(outputStream->stream->codecpar, codecContext);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not copy the stream parameters\n");
        exit(EXIT_FAILURE);
    }

    // Create resampler context.
    outputStream->swrContext = swr_alloc();
    if (!outputStream->swrContext) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate resampler context\n");
        exit(EXIT_FAILURE);
    }

    // Set options.
    av_opt_set_int(outputStream->swrContext, "in_channel_count", codecContext->channels, 0);
    av_opt_set_int(outputStream->swrContext, "in_sample_rate", codecContext->sample_rate, 0);
    av_opt_set_sample_fmt(outputStream->swrContext, "in_sample_fmt", AV_SAMPLE_FMT_S16, 0);
    av_opt_set_int(outputStream->swrContext, "out_channel_count", codecContext->channels, 0);
    av_opt_set_int(outputStream->swrContext, "out_sample_rate", codecContext->sample_rate, 0);
    av_opt_set_sample_fmt(outputStream->swrContext, "out_sample_fmt", codecContext->sample_fmt, 0);

    // Initialize the resampling context.
    if ((ret = swr_init(outputStream->swrContext)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Failed to initialize the resampling context\n");
        exit(ret);
    }
}

static int logCount = 0;

static void LogPacket(const AVFormatContext* formatContext, const AVPacket* packet) {
    logCount++;
    AVRational* timeBase = &formatContext->streams[packet->stream_index]->time_base;
    char packetPts[AV_TS_MAX_STRING_SIZE];
    char packetPtsTime[AV_TS_MAX_STRING_SIZE];
    char packetDts[AV_TS_MAX_STRING_SIZE];
    char packetDtsTime[AV_TS_MAX_STRING_SIZE];
    char duration[AV_TS_MAX_STRING_SIZE];
    char durationTime[AV_TS_MAX_STRING_SIZE];
    if (logCount > 20 && logCount < 25) {
        printf("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
               av_ts_make_string(packetPts, packet->pts),
               av_ts_make_time_string(packetPtsTime, reinterpret_cast<int64_t>(packetPts), timeBase),
               av_ts_make_string(packetDts, packet->dts),
               av_ts_make_time_string(packetDtsTime, reinterpret_cast<int64_t>(packetDts), timeBase),
               av_ts_make_string(duration, packet->duration),
               av_ts_make_time_string(durationTime, packet->duration, timeBase),
               packet->stream_index);
    }
}

static bool writeFrame(AVFormatContext* formatContext, AVCodecContext* codecContext,
                       AVStream* stream, AVFrame* frame) {
    int ret;

    // Send the frame to the encoder.
    ret = avcodec_send_frame(codecContext, frame);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Error sending a frame wo the encoder\n");
        exit(EXIT_FAILURE);
    }

    while (ret >= 0) {
        AVPacket packet = {nullptr};
        ret = avcodec_receive_packet(codecContext, &packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            break;
        else if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Error encoding a frame\n");
            exit(EXIT_FAILURE);
        }

        // Rescale output packet timestamp values from codec to stream timebase.
        av_packet_rescale_ts(&packet, codecContext->time_base, stream->time_base);
        // 设置packet的index
        packet.stream_index = stream->index;

        // Write the compressed frame to the media file.
        LogPacket(formatContext, &packet);
        ret = av_interleaved_write_frame(formatContext, &packet);
        av_packet_unref(&packet);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Error while writing output packet\n");
            exit(EXIT_FAILURE);
        }
    }
    return ret == AVERROR_EOF;
}

static void fillYUVImage(AVFrame* frame, int index, int width, int height) {
    // Y
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            frame->data[0][y * frame->linesize[0] + x] = x + y + index * 3;

    // Cb and Cr
    for (int y = 0; y < height / 2; y++) {
        for (int x = 0; x < width / 2; x++) {
            frame->data[1][y * frame->linesize[1] + x] = 128 + y + index * 2;
            frame->data[2][y * frame->linesize[2] + x] = 64 + x + index * 5;
        }
    }
}

static AVFrame* getVideoFrame(OutputStream* outputStream) {
    AVCodecContext* codecContext = outputStream->codecContext;

    // Check if we want to generate more frames.
    if (av_compare_ts(outputStream->nextPts, codecContext->time_base,
                      STREAM_DURATION, (AVRational) {1, 1}) > 0)
        return nullptr;

    // When we pass a frame to the encoder, it may keep a reference to it internally;
    // make sure we do not overwrite it here.
    if (av_frame_make_writable(outputStream->frame) < 0)
        exit(EXIT_FAILURE);

    if (codecContext->pix_fmt != AV_PIX_FMT_YUV420P) {
        // As we only generate a YUV420P picture, we must convert it
        // to the codec pixel format if need.
        if (!outputStream->swsContext) {
            outputStream->swsContext = sws_getContext(codecContext->width,
                                                      codecContext->height,
                                                      AV_PIX_FMT_YUV420P,
                                                      codecContext->width,
                                                      codecContext->height,
                                                      codecContext->pix_fmt,
                                                      SWS_BICUBIC,
                                                      nullptr, nullptr, nullptr);
            if (!outputStream->swsContext) {
                av_log(nullptr, AV_LOG_ERROR, "Could not initialize the conversion context\n");
                exit(EXIT_FAILURE);
            }
        }
        fillYUVImage(outputStream->frame, outputStream->nextPts, codecContext->width, codecContext->height);
        sws_scale(outputStream->swsContext, (const uint8_t* const*) outputStream->tmpFrame->data,
                  outputStream->tmpFrame->linesize, 0, codecContext->height, outputStream->frame->data,
                  outputStream->frame->linesize);
    } else {
        fillYUVImage(outputStream->frame, outputStream->nextPts, codecContext->width, codecContext->height);
    }
    outputStream->frame->pts = outputStream->nextPts++;
    return outputStream->frame;
}

// Encode one video frame and send it to the muxer return true when encoding is finished, 0 otherwise.
static bool writeVideoFrame(AVFormatContext* formatContext, OutputStream* outputStream) {
    return writeFrame(formatContext, outputStream->codecContext,
                      outputStream->stream, getVideoFrame(outputStream));
}

static AVFrame* getAudioFrame(OutputStream* outputStream) {
    AVFrame* frame = outputStream->tmpFrame;

    // Check if we want to generate more frames.
    if (av_compare_ts(outputStream->nextPts, outputStream->codecContext->time_base,
                      STREAM_DURATION, (AVRational) {1, 1}) > 0)
        return nullptr;

    auto* q = (int16_t*) frame->data[0];
    for (int j = 0; j < frame->nb_samples; j++) {
        int v = (int) (sin(outputStream->t) * 10000);
        for (int i = 0; i < outputStream->codecContext->channels; i++) {
            *q++ = v;
        }
        outputStream->t += outputStream->tincr;
        outputStream->tincr += outputStream->tincr2;
    }
    frame->pts = outputStream->nextPts;
    outputStream->nextPts += frame->nb_samples;
    return frame;
}

static bool writeAudioFrame(AVFormatContext* formatContext, OutputStream* outputStream) {
    AVCodecContext* codecContext = outputStream->codecContext;
    AVFrame* frame;
    int ret;
    int dstNumberSamples;

    frame = getAudioFrame(outputStream);
    if (frame) {
        dstNumberSamples = av_rescale_rnd(swr_get_delay(outputStream->swrContext,
                                                        codecContext->sample_rate) + frame->nb_samples,
                                          codecContext->sample_rate, codecContext->sample_rate, AV_ROUND_UP);
        av_assert0(dstNumberSamples == frame->nb_samples);

        ret = av_frame_make_writable(outputStream->frame);
        if (ret < 0)
            exit(EXIT_FAILURE);

        // Convert to destination format.
        ret = swr_convert(outputStream->swrContext,
                          outputStream->frame->data,
                          dstNumberSamples,
                          (const uint8_t**) frame->data, frame->nb_samples);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Error while converting\n");
            exit(EXIT_FAILURE);
        }

        frame = outputStream->frame;
        frame->pts = av_rescale_q(outputStream->samplesCount, (AVRational) {1, codecContext->sample_rate},
                                  codecContext->time_base);
        outputStream->samplesCount += dstNumberSamples;
    }
    return writeFrame(formatContext, codecContext, outputStream->stream, frame);
}

int main(int argc, char** argv) {
    OutputStream videoStream = {nullptr}, audioStream = {nullptr};

    const char* filename;
    AVOutputFormat* outputFormat;
    AVFormatContext* formatContext = nullptr;

    AVCodec* audioCodec, * videoCodec;

    int ret;
    bool hasVideo = false, hasAudio = false;
    bool encodeVideo = false, encodeAudio = false;
    AVDictionary* option = nullptr;

    if (argc < 2) {
        printf("Usage: %s output_file\n"
               "API example program to output a media file with libavformat.\n"
               "This program generates a synthetic audio and video stream, encodes and\n"
               "muxes them into a file name output file.\n"
               "This output format is automatically guessed according to the file extension.\n",
               argv[0]);
        return EXIT_FAILURE;
    }

    filename = argv[1];
    // Allocate the output media context.
    avformat_alloc_output_context2(&formatContext, nullptr, nullptr, filename);
    if (!formatContext) {
        av_log(nullptr, AV_LOG_ERROR, "Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&formatContext, nullptr, "mpeg", filename);
    }
    outputFormat = formatContext->oformat;

    // Add the audio and video stream using the default format codecs
    // and initialize the codecs.
    if (outputFormat->video_codec != AV_CODEC_ID_NONE) {
        addStream(&videoStream, formatContext, &videoCodec, outputFormat->video_codec);
        hasVideo = true;
        encodeVideo = true;
    }

    if (outputFormat->audio_codec != AV_CODEC_ID_NONE) {
        addStream(&audioStream, formatContext, &audioCodec, outputFormat->audio_codec);
        hasAudio = true;
        encodeAudio = true;
    }

    // Now that all the parameters are set, we can open the audio and
    // video codecs and allocate the necessary encode buffers.
    if (hasVideo)
        openVideo(videoCodec, &videoStream, option);
    if (hasAudio)
        openAudio(audioCodec, &audioStream, option);

    // av_dump_format(formatContext, 0, filename, true);

    // Open the output file, if needed.
    if (!((unsigned) outputFormat->flags & (unsigned) AVFMT_NOFILE)) {
        ret = avio_open(&formatContext->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Could not open %s\n", filename);
            return EXIT_FAILURE;
        }
    }

    // Write the stream header, if any.
    ret = avformat_write_header(formatContext, &option);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Error occurred when opening output file\n");
        return EXIT_FAILURE;
    }

    // 只有到EOF时encodeVideo和encodeAudio才为false
    while (encodeVideo || encodeAudio) {
        // Select the stream to encode.
        if (encodeVideo &&
            (!encodeAudio) || av_compare_ts(videoStream.nextPts, videoStream.codecContext->time_base,
                                            audioStream.nextPts, audioStream.codecContext->time_base) <= 0) {
            encodeVideo = !writeVideoFrame(formatContext, &videoStream);
        } else {
            encodeAudio = !writeAudioFrame(formatContext, &audioStream);
        }
    }

    return 0;
}
