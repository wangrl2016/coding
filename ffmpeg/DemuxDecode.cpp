//
// Created by wangrl on 2021/2/26.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/timestamp.h>
}

/**
 * 初始化解码器
 *
 * @param streamIndex           [out] 流下标值
 * @param decodeContext         [out] 解码器
 * @param inputFormatContext    [in]  容器
 * @param type                  [in]　类型
 * @return
 */
static int openCodecContext(int* streamIndex,
                            AVCodecContext** decodeContext, AVFormatContext* inputFormatContext,
                            enum AVMediaType type) {
    AVCodec* decoder = nullptr;
    AVStream* stream = nullptr;
    int ret, tmpStreamIndex;
    ret = av_find_best_stream(inputFormatContext, type, -1, -1, nullptr, 0);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find %s stream in input file\n",
               av_get_media_type_string(type));
        return ret;
    } else {
        tmpStreamIndex = ret;
        stream = inputFormatContext->streams[tmpStreamIndex];

        // Find decoder for the stream.
        decoder = avcodec_find_decoder(stream->codecpar->codec_id);
        if (!decoder) {
            av_log(nullptr, AV_LOG_ERROR, "Failed to find %s codec\n", av_get_media_type_string(type));
            return AVERROR(EINVAL);
        }

        // Allocate a codec context for the decoder.
        *decodeContext = avcodec_alloc_context3(decoder);
        if (!*decodeContext) {
            av_log(nullptr, AV_LOG_ERROR, "Failed to allocate the %s codec context\n",
                   av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }

        // Copy codec parameters from input stream to output codec context.
        if ((ret = avcodec_parameters_to_context(*decodeContext, stream->codecpar)) < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Failed to copy %s codec parameters to decoder context\n",
                   av_get_media_type_string(type));
            return ret;
        }

        // Init the decoders.
        if ((ret = avcodec_open2(*decodeContext, decoder, nullptr)) < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Failed to open %s codec\n",
                   av_get_media_type_string(type));
            return ret;
        }
        *streamIndex = tmpStreamIndex;
    }
    return 0;
}

static int decodePacket(AVCodecContext* codecContext, const AVPacket* pkt, AVFrame* frame) {
    int ret = 0;

    // Submit the packet to the decoder.
    ret = avcodec_send_packet(codecContext, pkt);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Error submitting a packet for decoding\n");
        return ret;
    }

    // Get all the available frames from the decoder.
    while (ret >= 0) {
        ret = avcodec_receive_frame(codecContext, frame);
        if (ret < 0) {
            if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
                return 0;

            av_log(nullptr, AV_LOG_ERROR, "Error during decoding\n");
            return ret;
        }

        // Write the frame data to output file.
        if (codecContext->codec->type == AVMEDIA_TYPE_VIDEO) {

        } else {

        }
        av_frame_unref(frame);
        if (ret < 0)
            return ret;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int ret = 0;

    if (argc != 4) {
        av_log(nullptr, AV_LOG_ERROR,
               "Usage: %s input_file video_output_file audio_output_file\n"
               "API example program to show how to read frames from an input file.\n"
               "This program reads frames from a file, decodes them, and writes decoded\n"
               "video frames to a rawvideo file named video_output_file, and decoded\n"
               "audio frames to a rawvideo file named audio_output_file.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* srcFilename = argv[1];
    char* videoDstFilename = argv[2];
    char* audioDstFilename = argv[3];

    AVFormatContext* inputFormatContext = nullptr;
    int videoStreamIndex, audioStreamIndex;
    AVCodecContext* videoDecodeContext = nullptr;
    AVCodecContext* audioDecodeContext = nullptr;
    AVStream* videoStream = nullptr, * audioStream = nullptr;

    FILE* videoDstFile, * audioDstFile;
    int width, height;
    AVPixelFormat pixelFormat;
    AVFrame* frame = nullptr;

    // 存放视频数据
    uint8_t* videoDstData[4] = {nullptr};
    int videoDstLinesize[4];
    int videoDstBufSize;

    AVPacket pkt;

    // Open input file, and allocate format context.
    if (avformat_open_input(&inputFormatContext, srcFilename, nullptr, nullptr) < 0) {
        fprintf(stderr, "Could not open source file %s\n", srcFilename);
        exit(EXIT_FAILURE);
    }

    // Retrieve stream information.
    if (avformat_find_stream_info(inputFormatContext, nullptr) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open source file %s\n", srcFilename);
        exit(EXIT_FAILURE);
    }

    if (openCodecContext(&videoStreamIndex, &videoDecodeContext, inputFormatContext, AVMEDIA_TYPE_VIDEO) >= 0) {
        videoStream = inputFormatContext->streams[videoStreamIndex];
        videoDstFile = fopen(videoDstFilename, "wb");
        if (!videoDstFile) {
            av_log(nullptr, AV_LOG_ERROR, "Could not open destination file %s\n", videoDstFilename);
            ret = 1;
            goto end;
        }
        // Allocate image where the decoded image will be put.
        width = videoDecodeContext->width;
        height = videoDecodeContext->height;
        pixelFormat = videoDecodeContext->pix_fmt;
        ret = av_image_alloc(videoDstData, videoDstLinesize,
                             width, height, pixelFormat, 1);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Could not allocate raw video buffer\n");
            goto end;
        }
        videoDstBufSize = ret;
    }

    if (openCodecContext(&audioStreamIndex, &audioDecodeContext, inputFormatContext, AVMEDIA_TYPE_AUDIO) >= 0) {
        audioStream = inputFormatContext->streams[audioStreamIndex];
        audioDstFile = fopen(audioDstFilename, "wb");
        if (!audioDstFile) {
            av_log(nullptr, AV_LOG_ERROR, "Could not open destination file %s\n", audioDstFilename);
            ret = 1;
            goto end;
        }
    }

    // Dump input formation to stderr.
    av_dump_format(inputFormatContext, 0, srcFilename, 0);

    if (!audioStream && !videoStream) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find audio or video stream in the input, aborting\n");
        ret = 1;
        goto end;
    }

    // Initialize packet, set data to nullptr, let the demuxer fill it.
    av_init_packet(&pkt);
    pkt.data = nullptr;
    pkt.size = 0;

    // Read frame file the file.
    while (av_read_frame(inputFormatContext, &pkt) >= 0) {
        // Check if the packet belongs to a stream we are interested in, otherwise skip it.
        if (pkt.stream_index == videoStreamIndex)
            ret = decodePacket(videoDecodeContext, &pkt, frame);
        else if (pkt.stream_index == audioStreamIndex)
            ret = decodePacket(audioDecodeContext, &pkt, frame);
        av_packet_unref(&pkt);
        if (ret < 0)
            break;
    }

    // Flush the decoders
    if (videoDecodeContext)
        decodePacket(videoDecodeContext, nullptr, nullptr);
    if (audioDecodeContext)
        decodePacket(audioDecodeContext, nullptr, nullptr);

    printf("Demuxing succeeded");

    end:
    avformat_close_input(&inputFormatContext);
    return ret < 0;
}
