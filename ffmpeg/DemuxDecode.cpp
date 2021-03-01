//
// Created by wangrl on 2021/2/26.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/timestamp.h>
}

static AVFormatContext* inputFormatContext = nullptr;
static AVCodecContext* videoDecodeContext = nullptr, * audioDecodeContext = nullptr;
static int width, height;
static enum AVPixelFormat pixelFormat;
static AVStream* videoStream = nullptr, * audioStream = nullptr;

static const char* srcFilename = nullptr;
static const char* videoDstFilename = nullptr;
static const char* audioDstFilename = nullptr;
static FILE* videoDstFile = nullptr;
static FILE* audioDstFile = nullptr;

static uint8_t* videoDstData[4] = {nullptr};
static int videoDstLinesize[4];
static int videoDstBufsize;

static int videoStreamIdx = -1, audioStreamIdx = -1;
static AVFrame* frame = nullptr;
static AVPacket pkt;

static int videoFrameCount = 0;
static int audioFrameCount = 0;

static int output_video_frame(AVFrame* frame) {
    if (frame->width != width || frame->height != height ||
        frame->format != pixelFormat) {
        /* To handle this change, one could call av_image_alloc again and
         * decode the following frames into another rawvideo file. */
        fprintf(stderr, "Error: Width, height and pixel format have to be "
                        "constant in a rawvideo file, but the width, height or "
                        "pixel format of the input video changed:\n"
                        "old: width = %d, height = %d, format = %s\n"
                        "new: width = %d, height = %d, format = %s\n",
                width, height, av_get_pix_fmt_name(pixelFormat),
                frame->width, frame->height,
                av_get_pix_fmt_name(static_cast<AVPixelFormat>(frame->format)));
        return -1;
    }
    videoFrameCount++;
    /* copy decoded frame to destination buffer:
     * this is required since rawvideo expects non aligned data */
    av_image_copy(videoDstData, videoDstLinesize,
                  (const uint8_t**) (frame->data), frame->linesize,
                  pixelFormat, width, height);
    /* write to rawvideo file */
    fwrite(videoDstData[0], 1, videoDstBufsize, videoDstFile);
    return 0;
}

static int output_audio_frame(AVFrame* frame) {
    size_t unpadded_linesize = frame->nb_samples *
                               av_get_bytes_per_sample(static_cast<AVSampleFormat>(frame->format));
    audioFrameCount++;
    /* Write the raw audio data samples of the first plane. This works
     * fine for packed formats (e.g. AV_SAMPLE_FMT_S16). However,
     * most audio decoders output planar audio, which uses a separate
     * plane of audio samples for each channel (e.g. AV_SAMPLE_FMT_S16P).
     * In other words, this code will write only the first audio channel
     * in these cases.
     * You should use libswresample or libavfilter to convert the frame
     * to packed data. */
    fwrite(frame->extended_data[0], 1, unpadded_linesize, audioDstFile);
    return 0;
}

static int decode_packet(AVCodecContext* dec, const AVPacket* pkt) {
    int ret = 0;
    // submit the packet to the decoder
    ret = avcodec_send_packet(dec, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error submitting a packet for decoding\n");
        return ret;
    }
    // get all the available frames from the decoder
    while (ret >= 0) {
        ret = avcodec_receive_frame(dec, frame);
        if (ret < 0) {
            // those two return values are special and mean there is no output
            // frame available, but there were no errors during decoding
            if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
                return 0;
            fprintf(stderr, "Error during decoding\n");
            return ret;
        }
        // write the frame data to output file
        if (dec->codec->type == AVMEDIA_TYPE_VIDEO)
            ret = output_video_frame(frame);
        else
            ret = output_audio_frame(frame);
        av_frame_unref(frame);
        if (ret < 0)
            return ret;
    }
    return 0;
}

static int open_codec_context(int* stream_idx,
                              AVCodecContext** dec_ctx, AVFormatContext* fmt_ctx, enum AVMediaType type) {
    int ret, stream_index;
    AVStream* st;
    AVCodec* dec = nullptr;
    AVDictionary* opts = nullptr;
    ret = av_find_best_stream(fmt_ctx, type, -1, -1, nullptr, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(type), srcFilename);
        return ret;
    } else {
        stream_index = ret;
        st = fmt_ctx->streams[stream_index];
        /* find decoder for the stream */
        dec = avcodec_find_decoder(st->codecpar->codec_id);
        if (!dec) {
            fprintf(stderr, "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            return AVERROR(EINVAL);
        }
        /* Allocate a codec context for the decoder */
        *dec_ctx = avcodec_alloc_context3(dec);
        if (!*dec_ctx) {
            fprintf(stderr, "Failed to allocate the %s codec context\n",
                    av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }
        /* Copy codec parameters from input stream to output codec context */
        if ((ret = avcodec_parameters_to_context(*dec_ctx, st->codecpar)) < 0) {
            fprintf(stderr, "Failed to copy %s codec parameters to decoder context\n",
                    av_get_media_type_string(type));
            return ret;
        }
        /* Init the decoders */
        if ((ret = avcodec_open2(*dec_ctx, dec, &opts)) < 0) {
            fprintf(stderr, "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            return ret;
        }
        *stream_idx = stream_index;
    }
    return 0;
}

static int get_format_from_sample_fmt(const char** fmt,
                                      enum AVSampleFormat sample_fmt) {
    int i;
    struct sample_fmt_entry {
        enum AVSampleFormat sample_fmt;
        const char* fmt_be, * fmt_le;
    } sample_fmt_entries[] = {
            {AV_SAMPLE_FMT_U8,  "u8",    "u8"},
            {AV_SAMPLE_FMT_S16, "s16be", "s16le"},
            {AV_SAMPLE_FMT_S32, "s32be", "s32le"},
            {AV_SAMPLE_FMT_FLT, "f32be", "f32le"},
            {AV_SAMPLE_FMT_DBL, "f64be", "f64le"},
    };
    *fmt = nullptr;
    for (i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++) {
        struct sample_fmt_entry* entry = &sample_fmt_entries[i];
        if (sample_fmt == entry->sample_fmt) {
            *fmt = AV_NE(entry->fmt_be, entry->fmt_le);
            return 0;
        }
    }
    fprintf(stderr,
            "sample format %s is not supported as output format\n",
            av_get_sample_fmt_name(sample_fmt));
    return -1;
}

int main(int argc, char** argv) {
    int ret = 0;
    if (argc != 4) {
        fprintf(stderr, "usage: %s  input_file video_output_file audio_output_file\n"
                        "API example program to show how to read frames from an input file.\n"
                        "This program reads frames from a file, decodes them, and writes decoded\n"
                        "video frames to a rawvideo file named video_output_file, and decoded\n"
                        "audio frames to a rawaudio file named audio_output_file.\n",
                argv[0]);
        exit(1);
    }
    srcFilename = argv[1];
    videoDstFilename = argv[2];
    audioDstFilename = argv[3];
    /* open input file, and allocate format context */
    if (avformat_open_input(&inputFormatContext, srcFilename, nullptr, nullptr) < 0) {
        fprintf(stderr, "Could not open source file %s\n", srcFilename);
        exit(1);
    }
    /* retrieve stream information */
    if (avformat_find_stream_info(inputFormatContext, nullptr) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        exit(1);
    }
    if (open_codec_context(&videoStreamIdx, &videoDecodeContext, inputFormatContext, AVMEDIA_TYPE_VIDEO) >= 0) {
        videoStream = inputFormatContext->streams[videoStreamIdx];
        videoDstFile = fopen(videoDstFilename, "wb");
        if (!videoDstFile) {
            fprintf(stderr, "Could not open destination file %s\n", videoDstFilename);
            ret = 1;
            goto end;
        }
        /* allocate image where the decoded image will be put */
        width = videoDecodeContext->width;
        height = videoDecodeContext->height;
        pixelFormat = videoDecodeContext->pix_fmt;
        ret = av_image_alloc(videoDstData, videoDstLinesize,
                             width, height, pixelFormat, 1);
        if (ret < 0) {
            fprintf(stderr, "Could not allocate raw video buffer\n");
            goto end;
        }
        videoDstBufsize = ret;
    }
    if (open_codec_context(&audioStreamIdx, &audioDecodeContext, inputFormatContext, AVMEDIA_TYPE_AUDIO) >= 0) {
        audioStream = inputFormatContext->streams[audioStreamIdx];
        audioDstFile = fopen(audioDstFilename, "wb");
        if (!audioDstFile) {
            fprintf(stderr, "Could not open destination file %s\n", audioDstFilename);
            ret = 1;
            goto end;
        }
    }
    /* dump input information to stderr */
    av_dump_format(inputFormatContext, 0, srcFilename, 0);
    if (!audioStream && !videoStream) {
        fprintf(stderr, "Could not find audio or video stream in the input, aborting\n");
        ret = 1;
        goto end;
    }
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate frame\n");
        ret = AVERROR(ENOMEM);
        goto end;
    }
    /* initialize packet, set data to nullptr, let the demuxer fill it */
    av_init_packet(&pkt);
    pkt.data = nullptr;
    pkt.size = 0;
    if (videoStream)
        printf("Demuxing video from file '%s' into '%s'\n", srcFilename, videoDstFilename);
    if (audioStream)
        printf("Demuxing audio from file '%s' into '%s'\n", srcFilename, audioDstFilename);
    /* read frames from the file */
    while (av_read_frame(inputFormatContext, &pkt) >= 0) {
        // check if the packet belongs to a stream we are interested in, otherwise
        // skip it
        if (pkt.stream_index == videoStreamIdx)
            ret = decode_packet(videoDecodeContext, &pkt);
        else if (pkt.stream_index == audioStreamIdx)
            ret = decode_packet(audioDecodeContext, &pkt);
        av_packet_unref(&pkt);
        if (ret < 0)
            break;
    }
    /* flush the decoders */
    if (videoDecodeContext)
        decode_packet(videoDecodeContext, nullptr);
    if (audioDecodeContext)
        decode_packet(audioDecodeContext, nullptr);
    printf("Demuxing succeeded.\n");
    if (videoStream) {
        printf("Play the output video file with the command:\n"
               "ffplay -f rawvideo -pix_fmt %s -video_size %dx%d %s\n",
               av_get_pix_fmt_name(pixelFormat), width, height,
               videoDstFilename);
    }
    if (audioStream) {
        enum AVSampleFormat sfmt = audioDecodeContext->sample_fmt;
        int n_channels = audioDecodeContext->channels;
        const char* fmt;
        if (av_sample_fmt_is_planar(sfmt)) {
            const char* packed = av_get_sample_fmt_name(sfmt);
            printf("Warning: the sample format the decoder produced is planar "
                   "(%s). This example will output the first channel only.\n",
                   packed ? packed : "?");
            sfmt = av_get_packed_sample_fmt(sfmt);
            n_channels = 1;
        }
        if ((ret = get_format_from_sample_fmt(&fmt, sfmt)) < 0)
            goto end;
        printf("Play the output audio file with the command:\n"
               "ffplay -f %s -ac %d -ar %d %s\n",
               fmt, n_channels, audioDecodeContext->sample_rate,
               audioDstFilename);
    }
    end:
    avcodec_free_context(&videoDecodeContext);
    avcodec_free_context(&audioDecodeContext);
    avformat_close_input(&inputFormatContext);
    if (videoDstFile)
        fclose(videoDstFile);
    if (audioDstFile)
        fclose(audioDstFile);
    av_frame_free(&frame);
    av_free(videoDstData[0]);
    return ret < 0;
}
