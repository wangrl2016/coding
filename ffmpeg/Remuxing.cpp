//
// Created by wangrl on 2021/3/4.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

int main(int argc, char** argv) {
    AVFormatContext* inputFormatContext = nullptr, * outputFormatContext = nullptr;
    AVPacket packet;

    if (argc < 3) {
        av_log(nullptr, AV_LOG_ERROR,
               "Usage: %s input output\n"
               "API example program to remux a media file with libavformat and libavcodec.\n"
               "The output format is guessed according to the file extension.\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* inputFilename = argv[1], * outputFilename = argv[2];

    int ret;
    if ((ret = avformat_open_input(&inputFormatContext, inputFilename, nullptr, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Could not open input file\n");
        return ret;
    }

    if ((ret = avformat_find_stream_info(inputFormatContext, 0)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Failed to retrieve input stream information\n");
        return ret;
    }

    av_dump_format(inputFormatContext, 0, inputFilename, false);

    avformat_alloc_output_context2(&outputFormatContext, nullptr, nullptr, outputFilename);
    if (!outputFormatContext) {
        av_log(nullptr, AV_LOG_ERROR, "Could not create output context\n");
        avformat_close_input(&inputFormatContext);
        return AVERROR_UNKNOWN;
    }

    int streamMappingSize = inputFormatContext->nb_streams;
    int* streamMapping = static_cast<int*>(av_mallocz_array(streamMappingSize, sizeof(*streamMapping)));

    AVOutputFormat* outputFormat = outputFormatContext->oformat;
    int streamIndex = 0;
    for (int i = 0; i < inputFormatContext->nb_streams; i++) {
        AVStream* outputStream;
        AVStream* inputStream = inputFormatContext->streams[i];
        AVCodecParameters* inputCodecPar = inputStream->codecpar;

        if (inputCodecPar->codec_type != AVMEDIA_TYPE_AUDIO &&
            inputCodecPar->codec_type != AVMEDIA_TYPE_VIDEO &&
            inputCodecPar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
            streamMapping[i] = -1;
            continue;
        }

        streamMapping[i] = streamIndex++;

        outputStream = avformat_new_stream(outputFormatContext, nullptr);
        if (!outputStream) {
            av_log(nullptr, AV_LOG_ERROR, "Failed allocating output stream\n");
            return AVERROR_UNKNOWN;
        }

        ret = avcodec_parameters_copy(outputStream->codecpar, inputCodecPar);
        outputStream->codecpar->codec_tag = 0;
    }

    av_dump_format(outputFormatContext, 0, outputFilename, true);

    if (!(outputFormat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&outputFormatContext->pb, outputFilename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            av_log(nullptr, AV_LOG_ERROR, "Could not open output file %s\n", outputFilename);
            return ret;
        }
    }

    ret = avformat_write_header(outputFormatContext, nullptr);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Error occurred when opening output file\n");
        return ret;
    }

    while (true) {
        AVStream* inStream, * outStream;

        ret = av_read_frame(inputFormatContext, &packet);
        if (ret < 0)
            break;

        inStream = inputFormatContext->streams[packet.stream_index];
        if (packet.stream_index >= streamMappingSize ||
            streamMapping[packet.stream_index] < 0) {
            av_packet_unref(&packet);
            continue;
        }

        packet.stream_index = streamMapping[packet.stream_index];
        outStream = outputFormatContext->streams[packet.stream_index];

        packet.pts = av_rescale_q_rnd(packet.pts, inStream->time_base, outStream->time_base,
                                      static_cast<AVRounding>(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        packet.dts = av_rescale_q_rnd(packet.dts, inStream->time_base, outStream->time_base,
                                      static_cast<AVRounding>(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        packet.pos = -1;

        ret = av_interleaved_write_frame(outputFormatContext, &packet);
        if (ret < 0)
            break;
        av_packet_unref(&packet);
    }
    av_write_trailer(outputFormatContext);

    avformat_close_input(&inputFormatContext);
    if (outputFormatContext && !(outputFormat->flags & AVFMT_NOFILE))
        avio_closep(&outputFormatContext->pb);

    avformat_free_context(outputFormatContext);
    av_freep(&streamMapping);

    return EXIT_SUCCESS;
}