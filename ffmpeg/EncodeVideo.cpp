//
// Created by wangrl on 2021/2/24.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}

#define STREAM_FRAME_RATE 25
#define STREAM_PIX_FMT AV_PIX_FMT_YUV422P
#define STREAM_DURATION 10.0

// pts of the next frame that will be generated.
int64_t NEXT_PTS;

static AVFrame* allocPicture(enum AVPixelFormat pixelFormat, int width, int height) {
    AVFrame* picture;
    int ret;
    picture = av_frame_alloc();
    if (!picture)
        return nullptr;

    picture->format = pixelFormat;
    picture->width = width;
    picture->height = height;

    ret = av_frame_get_buffer(picture, 0);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate frame data\n");
        exit(ret);
    }
    return picture;
}

// Prepare a dummy image.
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

static AVFrame* getVideoFrame(AVCodecContext* codecContext, SwsContext* swsContext,
                              AVFrame* frame, AVFrame* tmpFrame) {
    // Check if we want to generate more frames.
    if (av_compare_ts(NEXT_PTS, codecContext->time_base,
                      STREAM_DURATION, (AVRational) {1, 1}) > 0)
        return nullptr;

    // When we pass a frame to the encoder, it may keep a reference to it
    // internally; make sure we do not overwrite it here.
    if (av_frame_make_writable(frame) < 0)
        exit(1);

    if (codecContext->pix_fmt != AV_PIX_FMT_YUV420P) {
        if (!swsContext) {
            swsContext = sws_getContext(codecContext->width,
                                        codecContext->height,
                                        AV_PIX_FMT_YUV420P,
                                        codecContext->width, codecContext->height,
                                        codecContext->pix_fmt,
                                        SWS_BICUBIC,
                                        nullptr, nullptr, nullptr);
            if (!swsContext) {
                av_log(nullptr, AV_LOG_ERROR, "Could not initialize the conversion context\n");
                exit(EXIT_FAILURE);
            }
            fillYUVImage(tmpFrame, NEXT_PTS, codecContext->width, codecContext->height);
            sws_scale(swsContext, (const uint8_t* const*) tmpFrame->data,
                      tmpFrame->linesize, 0, codecContext->height,
                      frame->data, frame->linesize);
        }
    } else {
        fillYUVImage(frame, NEXT_PTS, codecContext->width, codecContext->height);
    }

    frame->pts = NEXT_PTS++;
    return frame;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s output_file\n"
               "API example program to output a media file with libavformat.\n"
               "This program generates a synthetic video stream, encodes and"
               "muxes them into a file named output_file.\n"
               "The output format is automatically guessed according to the file extension.\n",
               argv[0]);
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];

    AVFormatContext* formatContext;

    avformat_alloc_output_context2(&formatContext, nullptr, nullptr, filename);
    if (!formatContext) {
        av_log(nullptr, AV_LOG_ERROR,
               "Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&formatContext, nullptr, "mpeg", filename);
    }
    if (!formatContext)
        return EXIT_FAILURE;

    AVOutputFormat* fmt = formatContext->oformat;
    AVCodec* codec;
    AVStream* stream;
    AVCodecContext* codecContext;
    bool hasVideo = false;

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

        // 初始化AVCodecContext变量。
        codecContext->codec_id = fmt->video_codec;
        codecContext->bit_rate = 400000;
        // Resolution must be a multiple of two.
        codecContext->width = 352;
        codecContext->height = 288;

        stream->time_base = (AVRational) {1, STREAM_FRAME_RATE};
        codecContext->time_base = stream->time_base;

        // Emit one intra frame every twelve frames at most.
        codecContext->gop_size = 12;
        codecContext->pix_fmt = STREAM_PIX_FMT;

        if (codecContext->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
            // Just for testing, we also add B-frames.
            codecContext->max_b_frames = 2;
        }
        if (codecContext->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
            codecContext->mb_decision = 2;
        }

        // Some format want stream headers to be separate.
        if (formatContext->oformat->flags & AVFMT_GLOBALHEADER)
            codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

        hasVideo = true;
    }

    int ret;

    // Now that all the parameters are set, we can open the video
    // codecs and allocate the necessary encode buffers.
    if (hasVideo) {
        // Open the codec.
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

    AVFrame* frame = allocPicture(codecContext->pix_fmt, codecContext->width, codecContext->height);
    if (!frame) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate video frame\n");
        exit(EXIT_FAILURE);
    }

    // If the output format is not YUV420P, then a temporary YUV420P
    // picture is needed too. It is then converted to the required
    // output format.
    AVFrame* tmpFrame = nullptr;
    if (codecContext->pix_fmt != AV_PIX_FMT_YUV420P) {
        tmpFrame = allocPicture(AV_PIX_FMT_YUV420P, codecContext->width, codecContext->height);
        if (!tmpFrame) {
            av_log(nullptr, AV_LOG_ERROR, "Could not allocate temporary picture\n");
            exit(EXIT_FAILURE);
        }
    }

    av_dump_format(formatContext, 0, filename, true);

    // Open the output file, if needed.
    if (!(fmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&formatContext->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Could not open '%s'\n", filename);
            return ret;
        }
    }

    // Write the stream header, if any.
    ret = avformat_write_header(formatContext, nullptr);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Error occurred when opening output file\n");
        return ret;
    }

    SwsContext* swsContext = nullptr;

    while (getVideoFrame(codecContext, swsContext, frame, tmpFrame)) {
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

    // Write the trailer, if any. The trailer must be written before you
    // close the CodecContexts open when you wrote the header; otherwise
    // av_write_trailer() may try to use memory that was free on
    // av_codec_close().
    av_write_trailer(formatContext);

    avcodec_free_context(&codecContext);
    av_frame_free(&frame);
    av_frame_free(&tmpFrame);

    if (!(formatContext->flags & AVFMT_NOFILE)) {
        // Close the output file.
        avio_closep(&formatContext->pb);
    }

    // Free the stream.
    avformat_free_context(formatContext);

    return EXIT_SUCCESS;
}
