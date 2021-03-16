//
// Created by wangrl on 2021/2/25.
//

extern "C" {
#include <libavformat/avformat.h>
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

int main(int argc, char* argv[]) {

    AVFormatContext* inputFormatContext = nullptr;
    AVCodecContext* inputCodecContext = nullptr;
    AVCodec* inputCodec = nullptr;

    if (argc < 3) {
        av_log(nullptr, AV_LOG_ERROR, "Usage: %s input_mp3_file out_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* inputFilename = argv[1];
    char* outputFilename = argv[2];
    FILE* outFile = fopen(outputFilename, "wb");

    int error;
    if ((error = avformat_open_input(&inputFormatContext, inputFilename, nullptr, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open input file '%s'\n", inputFilename);
        inputFormatContext = nullptr;
        return error;
    }

    if ((error = avformat_find_stream_info(inputFormatContext, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open find stream info\n");
        avformat_close_input(&inputFormatContext);
        return error;
    }

    // Make sure that there is only one stream in the input file.
    if (inputFormatContext->nb_streams != 1) {
        av_log(nullptr, AV_LOG_ERROR, "Expected one audio input stream, but found %d\n",
               inputFormatContext->nb_streams);
        avformat_close_input(&inputFormatContext);
        return AVERROR_EXIT;
    }

    // Find a decoder for the audio stream.
    if (!(inputCodec = avcodec_find_decoder(inputFormatContext->streams[0]->codecpar->codec_id))) {
        av_log(nullptr, AV_LOG_ERROR, "Could not find input codec\n");
        avformat_close_input(&inputFormatContext);
        return AVERROR_EXIT;
    }

    // Allocate a new decoding context.
    inputCodecContext = avcodec_alloc_context3(inputCodec);
    if (!inputCodecContext) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate a decoding context\n");
        avformat_close_input(&inputFormatContext);
        return AVERROR(ENOMEM);
    }

    // Initialize the stream parameters with demuxer information.
    error = avcodec_parameters_to_context(inputCodecContext,
                                          inputFormatContext->streams[0]->codecpar);
    if (error < 0) {
        avformat_close_input(&inputFormatContext);
        avcodec_free_context(&inputCodecContext);
        return error;
    }

    // Open the decoder for the audio stream to use it later.
    if ((error = avcodec_open2(inputCodecContext, inputCodec, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open input codec\n");
        avcodec_free_context(&inputCodecContext);
        avformat_close_input(&inputFormatContext);
        return error;
    }

    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();
    int count = 0;
    while (av_read_frame(inputFormatContext, packet) >= 0 && count++ < 200) {
        int ret = avcodec_send_packet(inputCodecContext, packet);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Error submitting the packet to the decoder\n");
            exit(EXIT_FAILURE);
        }

        // Read all the output frames (in general there may be any number of them.
        while (ret >= 0) {
            ret = avcodec_receive_frame(inputCodecContext, frame);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                break;
            else if (ret < 0) {
                av_log(nullptr, AV_LOG_ERROR, "Error during decoding\n");
                exit(ret);
            }

            int dataSize = av_get_bytes_per_sample(inputCodecContext->sample_fmt);
            if (dataSize < 0) {
                av_log(nullptr, AV_LOG_ERROR, "Failed to calculate data size\n");
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < frame->nb_samples; i++)
                for (int ch = 0; ch < inputCodecContext->channels; ch++)
                    fwrite(frame->data[ch] + dataSize * i, 1, dataSize, outFile);
        }
    }

    enum AVSampleFormat sfmt = inputCodecContext->sample_fmt;
    if (av_sample_fmt_is_planar(sfmt)) {
        const char* packed = av_get_sample_fmt_name(sfmt);
        sfmt = av_get_packed_sample_fmt(sfmt);
    }
    const char* fmt;
    if ((error = get_format_from_sample_fmt(&fmt, sfmt)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Get audio format failed\n");
        goto end;
    }

    printf("Play the output audio file with the command:\n"
           "ffplay -f %s -ac %d -ar %d %s\n",
           fmt, inputCodecContext->channels, inputCodecContext->sample_rate,
           outputFilename);

    end:
    fclose(outFile);

    avcodec_free_context(&inputCodecContext);
    av_frame_free(&frame);
    av_packet_free(&packet);

    avformat_close_input(&inputFormatContext);

    return EXIT_SUCCESS;
}
