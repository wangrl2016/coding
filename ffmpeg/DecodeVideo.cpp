//
// Created by wangrl on 2021/2/23.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}

/**
 * Save frame in ppm file.
 *
 * @param frame     AVFrame decoded from media file
 * @param width     the frame width
 * @param height    the frame height
 * @param index     the frame index
 */
static void saveFrame(AVFrame* frame, int width, int height, int index) {
    FILE* fp;
    char filename[32];

    // open file
    sprintf(filename, "out/frame-%d.ppm", index);
    fp = fopen(filename, "wb");
    if (fp == nullptr) { return; }

    // write header
    fprintf(fp, "P6\n%d %d\n255\n", width, height);

    // write pixel data
    for (int y = 0; y < height; y++) {
        fwrite(frame->data[0] + y * frame->linesize[0], 1, width * 3, fp);
    }
    fclose(fp);
}

int main(int argc, char* argv[]) {
    AVFormatContext* formatContext = nullptr;
    AVCodecContext* codecContext = nullptr;
    AVCodecParameters* codecParameters = nullptr;
    AVCodec* codec = nullptr;

    if (argc < 2) {
        printf("Usage: %s movie_file\n", argv[0]);
        return EXIT_FAILURE;
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
    AVFrame* frameRGB = av_frame_alloc();
    frameRGB->format = AV_PIX_FMT_RGB24;
    frameRGB->width = codecContext->width;
    frameRGB->height = codecContext->height;
    av_frame_get_buffer(frameRGB, 24);

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

    int index = 0;
    // Seek到第5s进行解码
    int64_t timestamp = 5 * AV_TIME_BASE;
    av_seek_frame(formatContext, videoStreamIndex, timestamp, AVSEEK_FLAG_BACKWARD);
    while (av_read_frame(formatContext, packet) >= 0 && index < 5) {
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

                // convert the image from its native format to rgb
                sws_scale(swsContext, (uint8_t const* const*) frame->data,
                          frame->linesize, 0, codecContext->height,
                          frameRGB->data, frameRGB->linesize);

                if (index++ < 5)
                    saveFrame(frameRGB, codecContext->width, codecContext->height, index);
            }
        }
        // free the packet that was allocated by av_read_frame
        av_packet_unref(packet);
    }

    av_packet_free(&packet);
    av_frame_free(&frame);
    av_frame_free(&frameRGB);

    avcodec_free_context(&codecContext);
    sws_freeContext(swsContext);
    avformat_close_input(&formatContext);
    return 0;
}
