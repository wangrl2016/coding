//
// Created by wangrl on 2021/3/3.
//

#include <libavformat/avformat.h>

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


}