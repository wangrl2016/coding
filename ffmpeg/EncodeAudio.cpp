//
// Created by wangrl on 2021/2/25.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/timestamp.h>
#include <libswresample/swresample.h>
}

#define STREAM_DURATION  10.0

int64_t NEXT_PTS;

static void writeFrame(AVFormatContext* formatContext, AVCodecContext* c,
                       AVStream* st, AVFrame* frame) {
    int ret;
    // send the frame to the encoder
    ret = avcodec_send_frame(c, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame to the encoder\n");
        exit(1);
    }
    while (ret >= 0) {
        AVPacket pkt = {0};
        ret = avcodec_receive_packet(c, &pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            break;
        else if (ret < 0) {
            fprintf(stderr, "Error encoding a frame\n");
            exit(1);
        }
        // rescale output packet timestamp values from codec to stream timebase
        av_packet_rescale_ts(&pkt, c->time_base, st->time_base);
        pkt.stream_index = st->index;

        ret = av_interleaved_write_frame(formatContext, &pkt);
        av_packet_unref(&pkt);
        if (ret < 0) {
            fprintf(stderr, "Error while writing output packet\n");
            exit(1);
        }
    }
}

static AVFrame* allocAudioFrame(enum AVSampleFormat sampleFormat,
                                uint64_t channelLayout,
                                int sampleRate, int nbSamples) {
    AVFrame* frame = av_frame_alloc();
    int ret;
    if (!frame) {
        fprintf(stderr, "Error allocating an audio frame\n");
        exit(1);
    }
    frame->format = sampleFormat;
    frame->channel_layout = channelLayout;
    frame->sample_rate = sampleRate;
    frame->nb_samples = nbSamples;
    if (nbSamples) {
        ret = av_frame_get_buffer(frame, 0);
        if (ret < 0) {
            fprintf(stderr, "Error allocating an audio buffer\n");
            exit(1);
        }
    }
    return frame;
}

// Prepare a 16 bit dummy audio frame of 'frame_size' samples
// and 'nb_channels' channels.
static AVFrame* getAudioFrame(AVCodecContext* codecContext,
                              AVFrame* tmpFrame, float* t, float* tincr, float tincr2) {
    int j, i, v;
    int16_t* q = (int16_t*) tmpFrame->data[0];

    // Check if we want to generate more frames.
    if (av_compare_ts(NEXT_PTS, codecContext->time_base,
                      STREAM_DURATION, (AVRational) {1, 1}) > 0)
        return NULL;
    for (j = 0; j < tmpFrame->nb_samples; j++) {
        v = (int) (sin(*t) * 10000);
        for (i = 0; i < codecContext->channels; i++)
            *q++ = v;
        *t += *tincr;
        *tincr += tincr2;
    }
    tmpFrame->pts = NEXT_PTS;
    NEXT_PTS += tmpFrame->nb_samples;
    return tmpFrame;
}

int main(int argc, char** argv) {
    const char* filename;
    AVOutputFormat* fmt;
    AVFormatContext* formatContext;
    SwrContext* swrContext;
    AVStream* stream;

    AVCodec* codec;
    AVCodecContext* codecContext;

    AVFrame* tmpFrame, * dstFrame;

    int ret;
    bool hasAudio = false;
    float t = 0, tincr = 0, tincr2 = 0;

    if (argc < 2) {
        printf("Usage: %s output_file\n", argv[0]);
        return EXIT_FAILURE;
    }
    filename = argv[1];

    avformat_alloc_output_context2(&formatContext, nullptr, nullptr, filename);
    if (!formatContext)
        return EXIT_FAILURE;

    fmt = formatContext->oformat;

    if (fmt->audio_codec != AV_CODEC_ID_NONE) {
        int i;
        codec = avcodec_find_encoder(fmt->audio_codec);
        if (!codec) {
            fprintf(stderr, "Could not find encoder for '%s'\n",
                    avcodec_get_name(fmt->audio_codec));
            exit(1);
        }
        stream = avformat_new_stream(formatContext, nullptr);
        if (!stream) {
            fprintf(stderr, "Could not allocate stream\n");
            exit(1);
        }
        stream->id = formatContext->nb_streams - 1;
        codecContext = avcodec_alloc_context3(codec);
        if (!codecContext) {
            fprintf(stderr, "Could not alloc an encoding context\n");
            exit(1);
        }

        codecContext->sample_fmt = codec->sample_fmts ?
                                   codec->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
        codecContext->bit_rate = 64000;
        codecContext->sample_rate = 44100;
        if (codec->supported_samplerates) {
            codecContext->sample_rate = codec->supported_samplerates[0];
            for (i = 0; codec->supported_samplerates[i]; i++) {
                if (codec->supported_samplerates[i] == 44100)
                    codecContext->sample_rate = 44100;
            }
        }
        codecContext->channels = av_get_channel_layout_nb_channels(codecContext->channel_layout);
        codecContext->channel_layout = AV_CH_LAYOUT_STEREO;
        if (codec->channel_layouts) {
            codecContext->channel_layout = codec->channel_layouts[0];
            for (i = 0; codec->channel_layouts[i]; i++) {
                if (codec->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
                    codecContext->channel_layout = AV_CH_LAYOUT_STEREO;
            }
        }
        codecContext->channels = av_get_channel_layout_nb_channels(codecContext->channel_layout);
        stream->time_base = (AVRational) {1, codecContext->sample_rate};

        if (formatContext->oformat->flags & AVFMT_GLOBALHEADER)
            codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

        hasAudio = true;
    }

    if (hasAudio) {
        int nbSamples;
        ret = avcodec_open2(codecContext, codec, nullptr);
        if (ret < 0) {
            fprintf(stderr, "Could not open audio codec\n");
            exit(ret);
        }

        tincr = 2 * M_PI * 110.0 / codecContext->sample_rate;
        // Increment frequency by 110 Hz per second.
        tincr2 = 2 * M_PI * 110.0 / codecContext->sample_rate / codecContext->sample_rate;

        if (codecContext->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
            nbSamples = 10000;
        else
            nbSamples = codecContext->frame_size;
        dstFrame = allocAudioFrame(codecContext->sample_fmt, codecContext->channel_layout,
                                   codecContext->sample_rate, nbSamples);
        tmpFrame = allocAudioFrame(AV_SAMPLE_FMT_S16, codecContext->channel_layout,
                                   codecContext->sample_rate, nbSamples);
        // Copy the stream parameters to the muxer.
        ret = avcodec_parameters_from_context(stream->codecpar, codecContext);
        if (ret < 0) {
            fprintf(stderr, "Could not copy the stream parameters\n");
            exit(ret);
        }

        swrContext = swr_alloc();
        if (!swrContext) {
            fprintf(stderr, "Could not allocate resampler context\n");
            exit(1);
        }

        av_opt_set_int(swrContext, "in_channel_count", codecContext->channels, 0);
        av_opt_set_int(swrContext, "in_sample_rate", codecContext->sample_rate, 0);
        av_opt_set_sample_fmt(swrContext, "in_sample_fmt", AV_SAMPLE_FMT_S16, 0);
        av_opt_set_int(swrContext, "out_channel_count", codecContext->channels, 0);
        av_opt_set_int(swrContext, "out_sample_rate", codecContext->sample_rate, 0);
        av_opt_set_sample_fmt(swrContext, "out_sample_fmt", codecContext->sample_fmt, 0);

        if ((ret = swr_init(swrContext)) < 0) {
            fprintf(stderr, "Failed to initialize the resampling context\n");
            exit(ret);
        }
    }

    av_dump_format(formatContext, 0, filename, true);

    if (!(fmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&formatContext->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open '%s'\n", filename);
            return 1;
        }
    }

    // Write the stream header, if any.
    ret = avformat_write_header(formatContext, nullptr);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
        return ret;
    }

    int dstNbSamples, samplesCount = 0;
    while ((tmpFrame = getAudioFrame(codecContext, tmpFrame, &t, &tincr, tincr2))) {
        dstNbSamples = av_rescale_rnd(swr_get_delay(swrContext,
                                                    codecContext->sample_rate) + tmpFrame->nb_samples,
                                      codecContext->sample_rate, codecContext->sample_rate, AV_ROUND_UP);
        av_assert0(dstNbSamples == tmpFrame->nb_samples);
        ret = av_frame_make_writable(dstFrame);
        if (ret < 0)
            exit(1);
        ret = swr_convert(swrContext,
                          dstFrame->data, dstNbSamples,
                          (const uint8_t**) tmpFrame->data, tmpFrame->nb_samples);
        if (ret < 0) {
            fprintf(stderr, "Error while converting\n");
            exit(1);
        }
        dstFrame->pts = av_rescale_q(samplesCount,
                                     (AVRational) {1, codecContext->sample_rate}, codecContext->time_base);
        samplesCount += dstNbSamples;

        writeFrame(formatContext, codecContext, stream, dstFrame);
    }

    // Flush the queue.
    writeFrame(formatContext, codecContext, stream, nullptr);

    av_write_trailer(formatContext);

    if (hasAudio) {
        avcodec_free_context(&codecContext);
        av_frame_free(&tmpFrame);
        av_frame_free(&dstFrame);
        swr_free(&swrContext);
    }

    if (!(fmt->flags & AVFMT_NOFILE))
        avio_closep(&formatContext->pb);

    avformat_free_context(formatContext);

    return EXIT_SUCCESS;
}
