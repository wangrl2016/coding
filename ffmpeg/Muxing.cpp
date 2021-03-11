//
// Created by wangrl on 2021/3/4.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

#define STREAM_FRAME_RATE 24

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
    int i;
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
    outputStream->stream->id = formatContext->nb_streams - 1;
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
                for (int i = 0; (*codec)->supported_samplerates[i]; i++) {
                    if ((*codec)->supported_samplerates[i] == 44100)
                        codecContext->sample_rate = 44100;
                }
            }
            codecContext->channels = av_get_channel_layout_nb_channels(codecContext->channel_layout);
            codecContext->channel_layout = AV_CH_LAYOUT_STEREO;
            if ((*codec)->channel_layouts) {
                codecContext->channel_layout = (*codec)->channel_layouts[0];
                for (int i = 0; (*codec)->channel_layouts[i]; i++) {
                    if ((*codec)->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
                        codecContext->channel_layout = AV_CH_LAYOUT_STEREO;
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

static void openVideo(AVFormatContext* formatContext,
                      AVCodec* codec, OutputStream* outputStream, AVDictionary* option) {
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

static void openAudio(AVFormatContext* formatContext, AVCodec* codec, OutputStream* outputStream,
                      AVDictionary* option) {
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
    outputStream->tincr = 2 * M_PI * 110.0 / codecContext->sample_rate;
    outputStream->tincr2 = 2 * M_PI * 110.0 / codecContext->sample_rate / codecContext->sample_rate;

    if (codecContext->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
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
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    OutputStream videoStream = {0}, audioStream = {0};

    const char* filename;
    AVOutputFormat* outputFormat = nullptr;
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
        openVideo(formatContext, videoCodec, &videoStream, option);
    if (hasAudio)
        openAudio(formatContext, audioCodec, &audioStream, option);

    av_dump_format(formatContext, 0, filename, true);

    return 0;
}
