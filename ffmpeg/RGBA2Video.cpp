//
// Created by wangrl on 2021/2/24.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#define VIDEO_WIDTH 1280
#define VIDEO_HEIGHT 720
#define STREAM_FRAME_RATE 25
#define STREAM_PIX_FMT AV_PIX_FMT_YUV420P
#define STREAM_DURATION 10.0

// pts of the next frame that will be generated.
int64_t NEXT_PTS = 0;

static void fillRGBAImage(AVFrame* frame, int index, int width, int height) {

}

static AVFrame* getVideoFrame(AVCodecContext* codecContext, SwsContext* swsContext,
                              AVFrame* frame, AVFrame* rgbaFrame) {

    // Check if we want to generate more frames.
    if (av_compare_ts(NEXT_PTS, codecContext->time_base,
                      STREAM_DURATION, (AVRational) {1, 1}) > 0) {
        return nullptr;
    }

    if (av_frame_make_writable(frame) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Frame not writable\n");
        exit(EXIT_FAILURE);
    }

    fillRGBAImage(rgbaFrame, NEXT_PTS, codecContext->width, codecContext->height);
    sws_scale(swsContext, (const uint8_t* const*) rgbaFrame->data,
              rgbaFrame->linesize, 0, codecContext->height,
              frame->data, frame->linesize);

    frame->pts = NEXT_PTS++;
    return frame;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s output_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];

    AVFormatContext* formatContext = nullptr;
    AVCodecContext* codecContext = nullptr;
    AVCodec* codec = nullptr;
    AVStream* stream = nullptr;
    SwsContext* swsContext = nullptr;

    bool hasVideo = false;
    int ret;

    avformat_alloc_output_context2(&formatContext, nullptr, nullptr, filename);
    if (!formatContext) {
        av_log(nullptr, AV_LOG_ERROR,
               "Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&formatContext, nullptr, "mpeg", filename);
    }
    if (!formatContext)
        return EXIT_FAILURE;

    AVOutputFormat* fmt = formatContext->oformat;

    if (fmt->video_codec != AV_CODEC_ID_NONE) {
        // Find the encoder.
        codec = avcodec_find_encoder(fmt->video_codec);
        if (!codec) {
            av_log(nullptr, AV_LOG_ERROR, "Could not find encoder for '%s'\n",
                   avcodec_get_name(fmt->video_codec));
            exit(EXIT_FAILURE);
        }

        stream = avformat_new_stream(formatContext, nullptr);
        if (!stream) {
            av_log(nullptr, AV_LOG_ERROR, "Could not allocate stream\n");
            exit(EXIT_FAILURE);
        }
        stream->id = formatContext->nb_streams - 1;

        codecContext = avcodec_alloc_context3(codec);
        if (!codecContext) {
            av_log(nullptr, AV_LOG_ERROR, "Could not alloc an encoding context\n");
            exit(EXIT_FAILURE);
        }

        codecContext->codec_id = fmt->video_codec;
        // Resolution must be a multiple of two.
        codecContext->width = VIDEO_WIDTH;
        codecContext->height = VIDEO_HEIGHT;

        stream->time_base = (AVRational) {1, STREAM_FRAME_RATE};
        codecContext->time_base = stream->time_base;

        codecContext->gop_size = 12;
        codecContext->pix_fmt = STREAM_PIX_FMT;

        if (formatContext->oformat->flags & AVFMT_GLOBALHEADER)
            codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

        swsContext = sws_getContext(codecContext->width, codecContext->height,
                                    AV_PIX_FMT_RGBA,
                                    codecContext->width, codecContext->height,
                                    codecContext->pix_fmt,
                                    SWS_BICUBIC, nullptr, nullptr, nullptr);

        hasVideo = true;
    }

    if (hasVideo) {
        ret = avcodec_open2(codecContext, codec, nullptr);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Open the codec failed");
            exit(ret);
        }

        // Copy the stream parameters to the muxer.
        ret = avcodec_parameters_from_context(stream->codecpar, codecContext);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Could not copy the stream parameters\n");
            exit(ret);
        }
    }

    AVFrame* frame = av_frame_alloc();
    frame->format = codecContext->pix_fmt;
    frame->width = codecContext->width;
    frame->height = codecContext->height;
    ret = av_frame_get_buffer(frame, 0);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate frame data\n");
        exit(ret);
    }

    AVFrame* rgbaFrame = av_frame_alloc();
    rgbaFrame->format = AV_PIX_FMT_RGBA;
    rgbaFrame->width = codecContext->width;
    rgbaFrame->height = codecContext->height;
    ret = av_frame_get_buffer(rgbaFrame, 32);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate frame data\n");
        exit(ret);
    }

    av_dump_format(formatContext, 0, filename, true);

    if (!(fmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&formatContext->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Could not open '%s'\n", filename);
            return ret;
        }
    }

    ret = avformat_write_header(formatContext, nullptr);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Error occurred when opening output file\n");
        return ret;
    }

    while (getVideoFrame(codecContext, swsContext, frame, rgbaFrame)) {
        // Send the frame to the encoder
        ret = avcodec_send_frame(codecContext, frame);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Error sending a frame to the encoder\n");
            exit(ret);
        }

        while (ret >= 0) {
            AVPacket pkt = {0};
            ret = avcodec_receive_packet(codecContext, &pkt);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                break;
            else if (ret < 0) {
                av_log(nullptr, AV_LOG_ERROR, "Error encoding a frame\n");
                exit(ret);
            }

            // Rescale output packet timestamp values from codec to stream timebase.
            av_packet_rescale_ts(&pkt, codecContext->time_base, stream->time_base);
            pkt.stream_index = stream->index;

            // Write the compressed frame to the media file.
            ret = av_interleaved_write_frame(formatContext, &pkt);
            av_packet_unref(&pkt);
            if (ret < 0) {
                av_log(nullptr, AV_LOG_ERROR, "Error while writing output packet\n");
                exit(ret);
            }
        }
    }

    av_write_trailer(formatContext);

    avcodec_free_context(&codecContext);
    av_frame_free(&frame);
    av_frame_free(&rgbaFrame);

    if (!(formatContext->flags & AVFMT_NOFILE)) {
        // Close the output file.
        avio_closep(&formatContext->pb);
    }

    // Free the stream.
    avformat_free_context(formatContext);

    return EXIT_SUCCESS;
}
