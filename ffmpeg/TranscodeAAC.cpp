//
// Created by wangrl on 2021/3/3.
//

/**
 * Simple audio converter
 *
 * Convert an input audio file to AAC in an MP4 container using FFmpeg.
 * Formats other than MP4 are supported based on the output file extension.
 */

#include <stdio.h>

extern "C" {
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavcodec/avcodec.h"
#include "libavutil/audio_fifo.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/frame.h"
#include "libavutil/opt.h"
#include "libswresample/swresample.h"
}

// The output bit rate in bit/s.
#define OUTPUT_BIT_RATE 96000

// The number of output channels.
#define OUTPUT_CHANNELS 2

// Global timestamp for the audio frames.
static int64_t pts = 0;

/**
 * Open an input file and the required decoder.
 * @param      filename             File to be opened
 * @param[out] inputFormatContext Format context of opened file
 * @param[out] inputCodecContext  Codec context of opened file
 * @return Error code (0 if successful)
 */
static int openInputFile(const char* filename,
                         AVFormatContext** inputFormatContext,
                         AVCodecContext** inputCodecContext) {
    AVCodecContext* codecContext;
    AVCodec* codec;
    int error;

    // Open the input file to read from it.
    if ((error = avformat_open_input(inputFormatContext, filename, nullptr,
                                     nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open input file\n");
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
        av_log(nullptr, AV_LOG_ERROR, "Expected one audio input stream, but found %d\n",
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
 * Some of these parameters are based on the input file's parameters.
 * @param      filename              File to be opened
 * @param      inputCodecContext   Codec context of input file
 * @param[out] outputFormatContext Format context of output file
 * @param[out] outputCodecContext  Codec context of output file
 * @return Error code (0 if successful)
 */
static int openOutputFile(const char* filename,
                          AVCodecContext* inputCodecContext,
                          AVFormatContext** outputFormatContext,
                          AVCodecContext** outputCodecContext) {
    AVCodecContext* codecContext = nullptr;
    AVIOContext* outputIoContext = nullptr;
    AVStream* stream = nullptr;
    AVCodec* codec = nullptr;
    int error;
    // Open the output file to write to it.
    if ((error = avio_open(&outputIoContext, filename,
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
    (*outputFormatContext)->pb = outputIoContext;
    // Guess the desired container format based on the file extension.
    if (!((*outputFormatContext)->oformat = av_guess_format(nullptr, filename,
                                                            nullptr))) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find output file format\n");
        goto cleanup;
    }
    if (!((*outputFormatContext)->url = av_strdup(filename))) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate url.\n");
        error = AVERROR(ENOMEM);
        goto cleanup;
    }
    // Find the encoder to be used by its name.
    if (!(codec = avcodec_find_encoder(AV_CODEC_ID_AAC))) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find an AAC encoder.\n");
        goto cleanup;
    }
    // Create a new audio stream in the output file container.
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
    // Allow the use of the experimental AAC encoder. */
    codecContext->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
    // Set the sample rate for the container. */
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
    // Save the encoder context for easier access later.
    *outputCodecContext = codecContext;
    return 0;
    cleanup:
    avcodec_free_context(&codecContext);
    avio_closep(&(*outputFormatContext)->pb);
    avformat_free_context(*outputFormatContext);
    *outputFormatContext = nullptr;
    return error < 0 ? error : AVERROR_EXIT;
}

/**
 * Initialize one data packet for reading or writing.
 * @param packet Packet to be initialized
 */
static void initPacket(AVPacket* packet) {
    av_init_packet(packet);
    // Set the packet data and size so that it is recognized as being empty.
    packet->data = nullptr;
    packet->size = 0;
}

/**
 * Initialize one audio frame for reading from the input file.
 * @param[out] frame Frame to be initialized
 * @return Error code (0 if successful)
 */
static int initInputFrame(AVFrame** frame) {
    if (!(*frame = av_frame_alloc())) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate input frame\n");
        return AVERROR(ENOMEM);
    }
    return 0;
}

/**
 * Initialize the audio resampler based on the input and output codec settings.
 * If the input and output sample formats differ, a conversion is required
 * libswresample takes care of this, but requires initialization.
 * @param      inputCodecContext  Codec context of the input file
 * @param      outputCodecContext Codec context of the output file
 * @param[out] resamplerContext     Resample context for the required conversion
 * @return Error code (0 if successful)
 */
static int initResampler(AVCodecContext* inputCodecContext,
                         AVCodecContext* outputCodecContext,
                         SwrContext** resamplerContext) {
    int error;
    // Create a resampler context for the conversion.
    // Set the conversion parameters.
    // Default channel layouts based on the number of channels
    // are assumed for simplicity (they are sometimes not detected
    // properly by the demuxer and/or decoder).
    *resamplerContext = swr_alloc_set_opts(nullptr,
                                           av_get_default_channel_layout(outputCodecContext->channels),
                                           outputCodecContext->sample_fmt,
                                           outputCodecContext->sample_rate,
                                           av_get_default_channel_layout(inputCodecContext->channels),
                                           inputCodecContext->sample_fmt,
                                           inputCodecContext->sample_rate,
                                           0, nullptr);
    if (!*resamplerContext) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate resampler context\n");
        return AVERROR(ENOMEM);
    }
    // Perform a sanity check so that the number of converted samples is
    // not greater than the number of samples to be converted.
    // If the sample rates differ, this case has to be handled differently
    av_assert0(outputCodecContext->sample_rate == inputCodecContext->sample_rate);
    // Open the resampler with the specified parameters.
    if ((error = swr_init(*resamplerContext)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open resampler context\n");
        swr_free(resamplerContext);
        return error;
    }
    return 0;
}

/**
 * Initialize a FIFO buffer for the audio samples to be encoded.
 * @param[out] fifo                 Sample buffer
 * @param      outputCodecContext Codec context of the output file
 * @return Error code (0 if successful)
 */
static int initFifo(AVAudioFifo** fifo, AVCodecContext* outputCodecContext) {
    // Create the FIFO buffer based on the specified output sample format.
    if (!(*fifo = av_audio_fifo_alloc(outputCodecContext->sample_fmt,
                                      outputCodecContext->channels, 1))) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate FIFO\n");
        return AVERROR(ENOMEM);
    }
    return 0;
}

/**
 * Write the header of the output file container.
 * @param outputFormatContext Format context of the output file
 * @return Error code (0 if successful)
 */
static int writeOutputFileHeader(AVFormatContext* outputFormatContext) {
    int error;
    if ((error = avformat_write_header(outputFormatContext, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not write output file header\n");
        return error;
    }
    return 0;
}

/**
 * Decode one audio frame from the input file.
 * @param      frame                Audio frame to be decoded
 * @param      inputFormatContext Format context of the input file
 * @param      inputCodecContext  Codec context of the input file
 * @param[out] dataPresent         Indicates whether data has been decoded
 * @param[out] finished             Indicates whether the end of file has
 *                                  been reached and all data has been
 *                                  decoded. If this flag is false, there
 *                                  is more data to be decoded, i.e., this
 *                                  function has to be called again.
 * @return Error code (0 if successful)
 */
static int decodeAudioFrame(AVFrame* frame,
                            AVFormatContext* inputFormatContext,
                            AVCodecContext* inputCodecContext,
                            int* dataPresent, int* finished) {
    // Packet used for temporary storage.
    AVPacket input_packet;
    int error;
    initPacket(&input_packet);
    // Read one audio frame from the input file into a temporary packet.
    if ((error = av_read_frame(inputFormatContext, &input_packet)) < 0) {
        // If we are at the end of the file, flush the decoder below.
        if (error == AVERROR_EOF)
            *finished = 1;
        else {
            av_log(nullptr, AV_LOG_ERROR, "Could not read frame\n");
            return error;
        }
    }
    // Send the audio frame stored in the temporary packet to the decoder.
    // The input audio stream decoder is used to do this.
    if ((error = avcodec_send_packet(inputCodecContext, &input_packet)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not send packet for decoding\n");
        return error;
    }
    // Receive one frame from the decoder.
    error = avcodec_receive_frame(inputCodecContext, frame);
    // If the decoder asks for more data to be able to decode a frame,
    // return indicating that no data is present.
    if (error == AVERROR(EAGAIN)) {
        error = 0;
        goto cleanup;
        // If the end of the input file is reached, stop decoding.
    } else if (error == AVERROR_EOF) {
        *finished = 1;
        error = 0;
        goto cleanup;
    } else if (error < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not decode frame\n");
        goto cleanup;
        // Default case: Return decoded data.
    } else {
        *dataPresent = 1;
        goto cleanup;
    }
    cleanup:
    av_packet_unref(&input_packet);
    return error;
}

/**
 * Initialize a temporary storage for the specified number of audio samples.
 * The conversion requires temporary storage due to the different format.
 * The number of audio samples to be allocated is specified in frameSize.
 * @param[out] convertedInputSamples Array of converted samples. The
 *                                     dimensions are reference, channel
 *                                     (for multi-channel audio), sample.
 * @param      outputCodecContext    Codec context of the output file
 * @param      frameSize              Number of samples to be converted in
 *                                     each round
 * @return Error code (0 if successful)
 */
static int initConvertedSamples(uint8_t*** convertedInputSamples,
                                AVCodecContext* outputCodecContext,
                                int frameSize) {
    int error;
    // Allocate as many pointers as there are audio channels.
    // Each pointer will later point to the audio samples of the corresponding
    // channels (although it may be nullptr for interleaved formats).
    if (!(*convertedInputSamples = (uint8_t**) calloc(outputCodecContext->channels,
                                                      sizeof(**convertedInputSamples)))) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate converted input sample pointers\n");
        return AVERROR(ENOMEM);
    }
    // Allocate memory for the samples of all channels in one consecutive
    // block for convenience.
    if ((error = av_samples_alloc(*convertedInputSamples, nullptr,
                                  outputCodecContext->channels,
                                  frameSize,
                                  outputCodecContext->sample_fmt, 0)) < 0) {
        fprintf(stderr,
                "Could not allocate converted input samples\n");
        av_freep(&(*convertedInputSamples)[0]);
        free(*convertedInputSamples);
        return error;
    }
    return 0;
}

/**
 * Convert the input audio samples into the output sample format.
 * The conversion happens on a per-frame basis, the size of which is
 * specified by frameSize.
 * @param      inputData       Samples to be decoded. The dimensions are
 *                              channel (for multi-channel audio), sample.
 * @param[out] convertedData   Converted samples. The dimensions are channel
 *                              (for multi-channel audio), sample.
 * @param      frameSize       Number of samples to be converted
 * @param      resamplerContext Resample context for the conversion
 * @return Error code (0 if successful)
 */
static int convertSamples(const uint8_t** inputData,
                          uint8_t** convertedData, const int frameSize,
                          SwrContext* resamplerContext) {
    int error;
    // Convert the samples using the resampler.
    if ((error = swr_convert(resamplerContext,
                             convertedData, frameSize,
                             inputData, frameSize)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not convert input samples\n");
        return error;
    }
    return 0;
}

/**
 * Add converted input audio samples to the FIFO buffer for later processing.
 * @param fifo                    Buffer to add the samples to
 * @param convertedInputSamples Samples to be added. The dimensions are channel
 *                                (for multi-channel audio), sample.
 * @param frameSize              Number of samples to be converted
 * @return Error code (0 if successful)
 */
static int addSamplesToFifo(AVAudioFifo* fifo,
                            uint8_t** convertedInputSamples,
                            const int frameSize) {
    int error;
    // Make the FIFO as large as it needs to be to hold both,
    // the old and the new samples.
    if ((error = av_audio_fifo_realloc(fifo, av_audio_fifo_size(fifo) + frameSize)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not reallocate FIFO\n");
        return error;
    }
    // Store the new samples in the FIFO buffer.
    if (av_audio_fifo_write(fifo, (void**) convertedInputSamples,
                            frameSize) < frameSize) {
        av_log(nullptr, AV_LOG_ERROR, "Could not write data to FIFO\n");
        return AVERROR_EXIT;
    }
    return 0;
}

/**
 * Read one audio frame from the input file, decode, convert and store
 * it in the FIFO buffer.
 * @param      fifo                 Buffer used for temporary storage
 * @param      inputFormatContext Format context of the input file
 * @param      inputCodecContext  Codec context of the input file
 * @param      outputCodecContext Codec context of the output file
 * @param      resamplerContext    Resample context for the conversion
 * @param[out] finished             Indicates whether the end of file has
 *                                  been reached and all data has been
 *                                  decoded. If this flag is false,
 *                                  there is more data to be decoded,
 *                                  i.e., this function has to be called
 *                                  again.
 * @return Error code (0 if successful)
 */
static int readDecodeConvertAndStore(AVAudioFifo* fifo,
                                     AVFormatContext* inputFormatContext,
                                     AVCodecContext* inputCodecContext,
                                     AVCodecContext* outputCodecContext,
                                     SwrContext* resamplerContext,
                                     int* finished) {
    // Temporary storage of the input samples of the frame read from the file.
    AVFrame* inputFrame = nullptr;
    // Temporary storage for the converted input samples.
    uint8_t** convertedInputSamples = nullptr;
    int data_present = 0;
    int ret = AVERROR_EXIT;
    // Initialize temporary storage for one input frame.
    if (initInputFrame(&inputFrame))
        goto cleanup;
    // Decode one frame worth of audio samples.
    if (decodeAudioFrame(inputFrame, inputFormatContext,
                         inputCodecContext, &data_present, finished))
        goto cleanup;
    // If we are at the end of the file and there are no more samples
    // in the decoder which are delayed, we are actually finished.
    // This must not be treated as an error.
    if (*finished) {
        ret = 0;
        goto cleanup;
    }
    // If there is decoded data, convert and store it.
    if (data_present) {
        // Initialize the temporary storage for the converted input samples.
        if (initConvertedSamples(&convertedInputSamples, outputCodecContext,
                                 inputFrame->nb_samples))
            goto cleanup;
        // Convert the input samples to the desired output sample format.
        // This requires a temporary storage provided by convertedInputSamples.
        if (convertSamples((const uint8_t**) inputFrame->extended_data, convertedInputSamples,
                           inputFrame->nb_samples, resamplerContext))
            goto cleanup;
        // Add the converted input samples to the FIFO buffer for later processing.
        if (addSamplesToFifo(fifo, convertedInputSamples,
                             inputFrame->nb_samples))
            goto cleanup;
        ret = 0;
    }
    ret = 0;
    cleanup:
    if (convertedInputSamples) {
        av_freep(&convertedInputSamples[0]);
        free(convertedInputSamples);
    }
    av_frame_free(&inputFrame);
    return ret;
}

/**
 * Initialize one input frame for writing to the output file.
 * The frame will be exactly frameSize samples large.
 * @param[out] frame                Frame to be initialized
 * @param      outputCodecContext Codec context of the output file
 * @param      frameSize           Size of the frame
 * @return Error code (0 if successful)
 */
static int initOutputFrame(AVFrame** frame,
                           AVCodecContext* outputCodecContext,
                           int frameSize) {
    int error;
    // Create a new frame to store the audio samples. */
    if (!(*frame = av_frame_alloc())) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate output frame\n");
        return AVERROR_EXIT;
    }
    // Set the frame's parameters, especially its size and format.
    // av_frame_get_buffer needs this to allocate memory for the
    // audio samples of the frame.
    // Default channel layouts based on the number of channels
    // are assumed for simplicity.
    (*frame)->nb_samples = frameSize;
    (*frame)->channel_layout = outputCodecContext->channel_layout;
    (*frame)->format = outputCodecContext->sample_fmt;
    (*frame)->sample_rate = outputCodecContext->sample_rate;
    // Allocate the samples of the created frame. This call will make
    // sure that the audio frame can hold as many samples as specified.
    if ((error = av_frame_get_buffer(*frame, 0)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate output frame samples\n");
        av_frame_free(frame);
        return error;
    }
    return 0;
}

/**
 * Encode one frame worth of audio to the output file.
 * @param      frame                 Samples to be encoded
 * @param      outputFormatContext Format context of the output file
 * @param      outputCodecContext  Codec context of the output file
 * @param[out] dataPresent          Indicates whether data has been
 *                                   encoded
 * @return Error code (0 if successful)
 */
static int encodeAudioFrame(AVFrame* frame,
                            AVFormatContext* outputFormatContext,
                            AVCodecContext* outputCodecContext,
                            int* dataPresent) {
    // Packet used for temporary storage. */
    AVPacket outputPacket;
    int error;
    initPacket(&outputPacket);
    // Set a timestamp based on the sample rate for the container.
    if (frame) {
        frame->pts = pts;
        pts += frame->nb_samples;
    }
    // Send the audio frame stored in the temporary packet to the encoder.
    // The output audio stream encoder is used to do this.
    error = avcodec_send_frame(outputCodecContext, frame);
    // The encoder signals that it has nothing more to encode.
    if (error == AVERROR_EOF) {
        error = 0;
        goto cleanup;
    } else if (error < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not send packet for encoding\n");
        return error;
    }
    // Receive one encoded frame from the encoder.
    error = avcodec_receive_packet(outputCodecContext, &outputPacket);
    // If the encoder asks for more data to be able to provide an
    // encoded frame, return indicating that no data is present.
    if (error == AVERROR(EAGAIN)) {
        error = 0;
        goto cleanup;
        // If the last frame has been encoded, stop encoding.
    } else if (error == AVERROR_EOF) {
        error = 0;
        goto cleanup;
    } else if (error < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not encode frame\n");
        goto cleanup;
        // Default case: Return encoded data.
    } else {
        *dataPresent = 1;
    }
    // Write one audio frame from the temporary packet to the output file.
    if (*dataPresent &&
        (error = av_write_frame(outputFormatContext, &outputPacket)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not write frame\n");
        goto cleanup;
    }
    cleanup:
    av_packet_unref(&outputPacket);
    return error;
}

/**
 * Load one audio frame from the FIFO buffer, encode and write it to the
 * output file.
 * @param fifo                  Buffer used for temporary storage
 * @param outputFormatContext Format context of the output file
 * @param outputCodecContext  Codec context of the output file
 * @return Error code (0 if successful)
 */
static int loadEncodeAndWrite(AVAudioFifo* fifo,
                              AVFormatContext* outputFormatContext,
                              AVCodecContext* outputCodecContext) {
    // Temporary storage of the output samples of the frame written to the file.
    AVFrame* outputFrame;
    // Use the maximum number of possible samples per frame.
    // If there is less than the maximum possible frame size in the FIFO
    // buffer use this number. Otherwise, use the maximum possible frame size.
    const int frameSize = FFMIN(av_audio_fifo_size(fifo),
                                outputCodecContext->frame_size);
    int dataWritten;
    // Initialize temporary storage for one output frame.
    if (initOutputFrame(&outputFrame, outputCodecContext, frameSize))
        return AVERROR_EXIT;
    // Read as many samples from the FIFO buffer as required to fill the frame.
    // The samples are stored in the frame temporarily.
    if (av_audio_fifo_read(fifo, (void**) outputFrame->data, frameSize) < frameSize) {
        av_log(nullptr, AV_LOG_ERROR, "Could not read data from FIFO\n");
        av_frame_free(&outputFrame);
        return AVERROR_EXIT;
    }
    // Encode one frame worth of audio samples.
    if (encodeAudioFrame(outputFrame, outputFormatContext,
                         outputCodecContext, &dataWritten)) {
        av_frame_free(&outputFrame);
        return AVERROR_EXIT;
    }
    av_frame_free(&outputFrame);
    return 0;
}

/**
 * Write the trailer of the output file container.
 * @param outputFormatContext Format context of the output file
 * @return Error code (0 if successful)
 */
static int writeOutputFileTrailer(AVFormatContext* outputFormatContext) {
    int error;
    if ((error = av_write_trailer(outputFormatContext)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not write output file\n");
        return error;
    }
    return 0;
}

int main(int argc, char** argv) {
    AVFormatContext* inputFormatContext = nullptr, * outputFormatContext = nullptr;
    AVCodecContext* inputCodecContext = nullptr, * outputCodecContext = nullptr;
    SwrContext* resamplerContext = nullptr;
    AVAudioFifo* fifo = nullptr;
    int ret = AVERROR_EXIT;
    if (argc != 3) {
        av_log(nullptr, AV_LOG_ERROR, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }
    // Open the input file for reading.
    if (openInputFile(argv[1], &inputFormatContext,
                      &inputCodecContext))
        goto cleanup;
    // Open the output file for writing.
    if (openOutputFile(argv[2], inputCodecContext,
                       &outputFormatContext, &outputCodecContext))
        goto cleanup;
    // Initialize the resampler to be able to convert audio sample formats.
    if (initResampler(inputCodecContext, outputCodecContext,
                      &resamplerContext))
        goto cleanup;
    // Initialize the FIFO buffer to store audio samples to be encoded.
    if (initFifo(&fifo, outputCodecContext))
        goto cleanup;
    // Write the header of the output file container.
    if (writeOutputFileHeader(outputFormatContext))
        goto cleanup;
    // Loop as long as we have input samples to read or output samples
    // to write; abort as soon as we have neither.
    while (true) {
        // Use the encoder's desired frame size for processing.
        const int output_frame_size = outputCodecContext->frame_size;
        int finished = 0;
        // Make sure that there is one frame worth of samples in the FIFO
        // buffer so that the encoder can do its work.
        // Since the decoder's and the encoder's frame size may differ, we
        // need to FIFO buffer to store as many frames worth of input samples
        // that they make up at least one frame worth of output samples.
        while (av_audio_fifo_size(fifo) < output_frame_size) {
            // Decode one frame worth of audio samples, convert it to the
            // output sample format and put it into the FIFO buffer.
            if (readDecodeConvertAndStore(fifo, inputFormatContext,
                                          inputCodecContext,
                                          outputCodecContext,
                                          resamplerContext, &finished))
                goto cleanup;
            // If we are at the end of the input file, we continue
            // encoding the remaining audio samples to the output file.
            if (finished)
                break;
        }
        // If we have enough samples for the encoder, we encode them.
        // At the end of the file, we pass the remaining samples to
        // the encoder.
        while (av_audio_fifo_size(fifo) >= output_frame_size ||
               (finished && av_audio_fifo_size(fifo) > 0))
            // Take one frame worth of audio samples from the FIFO buffer,
            // encode it and write it to the output file.
            if (loadEncodeAndWrite(fifo, outputFormatContext,
                                   outputCodecContext))
                goto cleanup;
        // If we are at the end of the input file and have encoded
        // all remaining samples, we can exit this loop and finish.
        if (finished) {
            int data_written;
            // Flush the encoder as it may have delayed frames.
            do {
                data_written = 0;
                if (encodeAudioFrame(nullptr, outputFormatContext,
                                     outputCodecContext, &data_written))
                    goto cleanup;
            } while (data_written);
            break;
        }
    }
    // Write the trailer of the output file container.
    if (writeOutputFileTrailer(outputFormatContext))
        goto cleanup;
    ret = 0;
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
