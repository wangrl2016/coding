//
// Created by wangrl on 2021/3/3.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

/**
 * Open an audio input file and the required decoder.
 *
 * @param filename              File to be opened
 * @param inputFormatContext    Format context of opened file
 * @param inputCodecContext     Codec context of opened file
 * @return error code (0 if successful)
 */
static int OpenAudioInputFile(const char* filename,
                              AVFormatContext** inputFormatContext,
                              AVCodecContext** inputCodecContext) {
    AVCodecContext* context;
    AVCodec* codec;
    int error;

    // Open the input file to read from it.
    if ((error = avformat_open_input(inputFormatContext, filename, nullptr,
                                     nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open input file '%s'\n", filename);
        *inputFormatContext = nullptr;
        return error;
    }

    // Get information on the input file (number os stream etc).
    if ((error = avformat_find_stream_info(*inputFormatContext, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open find stream info\n");
        return error;
    }

    // Make sure that there is only one stream in the input file.
    if ((*inputFormatContext)->nb_streams != 1) {
        av_log(nullptr, AV_LOG_ERROR, "Expected one audio input stream, but found %d\n",
               (*inputFormatContext)->nb_streams);
        avformat_close_input(inputFormatContext);
        return AVERROR_EXIT;
    }

    // Find a decoder for audio stream.
    if (!(codec = avcodec_find_decoder((*inputFormatContext)->streams[0]->codecpar->codec_id))) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find input codec\n");
        avformat_close_input(inputFormatContext);
        return AVERROR_EXIT;
    }

    // Allocate a new decoding context.
    context = avcodec_alloc_context3(codec);
    if (!context) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate a decoding context\n");
        avformat_close_input(inputFormatContext);
        return AVERROR(ENOMEM);
    }

    // Initialize the stream parameters with demuxer information.
    error = avcodec_parameters_to_context(context, (*inputFormatContext)->streams[0]->codecpar);
    if (error < 0) {
        avformat_close_input(inputFormatContext);
        avcodec_free_context(&context);
        return error;
    }

    // Open the decoder for the audio stream to use it later.
    if ((error = avcodec_open2(context, codec, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open input codec\n");
        avcodec_free_context(&context);
        avformat_close_input(inputFormatContext);
        return error;
    }

    // Save the decoder context for easier access later.
    *inputCodecContext = context;

    return 0;
}

static int OpenVideoInputFile(const char* filename,
                              AVFormatContext** inputFormatContext,
                              AVCodecContext** inputCodecContext,
                              int* index) {
    int error;
    if ((error = avformat_open_input(inputFormatContext, filename, nullptr, nullptr)) != 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open video file\n");
        *inputFormatContext = nullptr;
        return error;
    }

    // Get information on the input file (number os stream etc).
    if ((error = avformat_find_stream_info(*inputFormatContext, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open find stream info\n");
        return error;
    }

    int videoStreamIndex = -1;
    for (int i = 0; i < (*inputFormatContext)->nb_streams; i++) {
        if ((*inputFormatContext)->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }
    if (videoStreamIndex == -1) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find video stream\n");
        avformat_close_input(inputFormatContext);
        return AVERROR_EXIT;
    }

    AVCodecParameters* codecParameters =
            (*inputFormatContext)->streams[videoStreamIndex]->codecpar;
    AVCodec* codec = avcodec_find_decoder(codecParameters->codec_id);
    if (!codec) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find input codec\n");
        avformat_close_input(inputFormatContext);
        return AVERROR_EXIT;
    }

    AVCodecContext* context = avcodec_alloc_context3(codec);
    if (avcodec_parameters_to_context(context, codecParameters)) {
        fprintf(stderr, "could not copy codec context\n");
        return -1;
    }

    // Initialize the stream parameters with demuxer information.
    error = avcodec_parameters_to_context(context,
                                          (*inputFormatContext)->streams[videoStreamIndex]->codecpar);
    if (error < 0) {
        avformat_close_input(inputFormatContext);
        avcodec_free_context(&context);
        return error;
    }

    // Open the decoder for the audio stream to use it later.
    if ((error = avcodec_open2(context, codec, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open input codec\n");
        avcodec_free_context(&context);
        avformat_close_input(inputFormatContext);
        return error;
    }

    // Save the decoder context for easier access later.
    *inputCodecContext = context;

    // Save the video stream index.
    *index = videoStreamIndex;

    return 0;
}
