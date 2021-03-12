//
// Created by wangrl on 2021/3/3.
//

/**
 * Simple audio converter
 *
 * Convert an input audio file to AAC in an MP4 container using FFmpeg.
 * Formats other than MP4 are supported based on the output file extension.
 */

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
#include <libavutil/audio_fifo.h>
}

// The output bit rate in bit/s.
#define OUTPUT_BIT_RATE 96000

// The number of output channels.
#define OUTPUT_CHANNELS 2

/**
 * Open an input file and the required decoder.
 *
 * @param filename              File to be opened
 * @param inputFormatContext    Format context of opened file
 * @param inputCodecContext     Codec context of opened file
 * @return Error code (0 if successful)
 */
static int openInputFile(const char* filename,
                         AVFormatContext** inputFormatContext,
                         AVCodecContext** inputCodecContext) {
    int error;
    AVCodec* codec;
    AVCodecContext* codecContext;
    // Open the input file to read from it.
    if ((error = avformat_open_input(inputFormatContext, filename, nullptr, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open input file %s\n", filename);
        *inputFormatContext = nullptr;
        return error;
    }

    // Get information on the input file (number of streams etc.).
    if ((error = avformat_find_stream_info(*inputFormatContext, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open find stream info\n");
        avformat_close_input(inputFormatContext);
        return error;
    }

    // Make sure that there is only one stream in the input file.
    if ((*inputFormatContext)->nb_streams != 1) {
        av_log(nullptr, AV_LOG_ERROR, "Expected one audio input stream, but fount %d\n",
               (*inputFormatContext)->nb_streams);
        avformat_close_input(inputFormatContext);
        return AVERROR_EXIT;
    }

    // Find a decoder for the audio stream.
    if (!(codec = avcodec_find_decoder((*inputFormatContext)->streams[0]->codecpar->codec_id))) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find input codec\n");
        avformat_close_input(inputFormatContext);
        return AVERROR_EXIT;
    }

    // Allocate a new decoding context.
    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate a decoding context\n");
        avformat_close_input(inputFormatContext);
        return AVERROR(ENOMEM);
    }

    // Initialize the stream parameters with demuxer information.
    error = avcodec_parameters_to_context(codecContext, (*inputFormatContext)->streams[0]->codecpar);
    if (error < 0) {
        avformat_close_input(inputFormatContext);
        avcodec_free_context(&codecContext);
        return error;
    }

    // Open the decoder for the audio stream to use it later.
    if ((error = avcodec_open2(codecContext, codec, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open input codec\n");
        avcodec_free_context(&codecContext);
        avformat_close_input(inputFormatContext);
        return error;
    }

    // Save the decoder context for easier access later.
    *inputCodecContext = codecContext;

    return 0;
}

/**
 * Open an output file and the required encoder.
 * Also set some basic encoder parameters.
 *
 * @param filename              File to be opened
 * @param inputCodecContext     Codec context of input file
 * @param outputFormatContext   Format context of output file
 * @param outputCodecContext    Codec context of output file
 * @return Error code (0 if successful)
 */
static int openOutputFile(const char* filename,
                          AVCodecContext* inputCodecContext,
                          AVFormatContext** outputFormatContext,
                          AVCodecContext** outputCodecContext) {
    AVCodecContext* codecContext = nullptr;
    AVIOContext* outputIOContext = nullptr;
    AVStream* stream = nullptr;
    AVCodec* codec = nullptr;
    int error;

    // Open the output file to write to it.
    if ((error = avio_open(&outputIOContext, filename,
                           AVIO_FLAG_WRITE)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open output file\n");
        return error;
    }

    // Create a new format context for the output container format.
    if (!(*outputFormatContext = avformat_alloc_context())) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate output format context\n");
        return AVERROR(ENOMEM);
    }

    // Associate the output file (pointer) with the container format context.
    (*outputFormatContext)->pb = outputIOContext;

    // Guess the desired container format based on the file extension.
    if (!((*outputFormatContext)->oformat = av_guess_format(nullptr, filename, nullptr))) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find output file format\n");
        goto cleanup;
    }

    if (!((*outputFormatContext)->url = av_strdup(filename))) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate url\n");
        error = AVERROR(ENOMEM);
        goto cleanup;
    }

    // Find the encoder to be used by its name.
    if (!(codec = avcodec_find_encoder(AV_CODEC_ID_AAC))) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find an AAC encoder\n");
        goto cleanup;
    }

    // Create a new stream in the output file container.
    if (!(stream = avformat_new_stream(*outputFormatContext, nullptr))) {
        av_log(nullptr, AV_LOG_ERROR, "Could not create new stream\n");
        error = AVERROR(ENOMEM);
        goto cleanup;
    }

    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate an encoding context\n");
        error = AVERROR(ENOMEM);
        goto cleanup;
    }

    // Set the basic encoder parameters.
    // The input file's sample rate is used to avoid a sample rate conversion.
    codecContext->channels = OUTPUT_CHANNELS;
    codecContext->channel_layout = av_get_default_channel_layout(OUTPUT_CHANNELS);
    codecContext->sample_rate = inputCodecContext->sample_rate;
    codecContext->sample_fmt = codec->sample_fmts[0];
    codecContext->bit_rate = OUTPUT_BIT_RATE;

    // Allow the use of the experimental AAC encoder.
    codecContext->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;

    // Set the sample rate for the container.
    stream->time_base.den = inputCodecContext->sample_rate;
    stream->time_base.num = 1;

    // Some container formats (like MP4) require global headers to be present.
    // Mark the encoder so that it behaves accordingly.
    if ((*outputFormatContext)->oformat->flags & AVFMT_GLOBALHEADER)
        codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    // Open the encoder for the audio stream to use it later.
    if ((error = avcodec_open2(codecContext, codec, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open output codec\n");
        goto cleanup;
    }

    error = avcodec_parameters_from_context(stream->codecpar, codecContext);
    if (error < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not initialize stream parameters\n");
        goto cleanup;
    }

    // Save the encoder content for easier access later.
    *outputCodecContext = codecContext;
    return 0;

    cleanup:
    avcodec_free_context(&codecContext);
    avio_closep(&(*outputFormatContext)->pb);
    avformat_free_context(*outputFormatContext);
    *outputFormatContext = nullptr;
    return error < 0 ? error : AVERROR_EXIT;
}

int main(int argc, char** argv) {
    AVFormatContext* inputFormatContext = nullptr, * outputFormatContext = nullptr;
    AVCodecContext* inputCodecContext = nullptr, * outputCodecContext = nullptr;
    SwrContext* resamplerContext = nullptr;
    AVAudioFifo* fifo = nullptr;
    int ret = AVERROR_EXIT;

    if (argc != 3) {
        av_log(nullptr, AV_LOG_ERROR, "Usage: %s input_file output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the input file for reading.
    if (openInputFile(argv[1], &inputFormatContext, &inputCodecContext))
        goto cleanup;

    // Open the output file for writing.
    if (openOutputFile(argv[2], inputCodecContext,
                       &outputFormatContext, &outputCodecContext))
        goto cleanup;

    // Initialize the FIFO buffer to store audio samples to be encoded.

    cleanup:
    if (fifo)
        av_audio_fifo_free(fifo);
    swr_free(&resamplerContext);
    if (outputCodecContext)
        avcodec_free_context(&outputCodecContext);
    if (outputFormatContext) {
        avio_closep(&outputFormatContext->pb);
        avformat_free_context(outputFormatContext);
    }

    if (inputCodecContext)
        avcodec_free_context(&inputCodecContext);
    if (inputFormatContext)
        avformat_close_input(&inputFormatContext);

    return ret;
}
