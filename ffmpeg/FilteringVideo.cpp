//
// Created by wangrl on 2021/3/2.
//

extern "C" {
#include <libavformat/avformat.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersrc.h>
#include <libavfilter/buffersink.h>
#include <libavutil/opt.h>
}

const char* FILTER_DESCR = "scale=780:240,transpose=cclock";

static void pgmSave(AVFrame* frame, const char* filename) {
    FILE* file = fopen(filename, "wb");
    av_log(nullptr, AV_LOG_INFO, "Filename %s\n", filename);
    fprintf(file, "P5\n%d %d\n%d\n", frame->width, frame->height, 255);
    for (int i = 0; i < frame->height; i++)
        fwrite(frame->data[0] + i * frame->linesize[0], 1, frame->width, file);
    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        av_log(nullptr, AV_LOG_ERROR, "Usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    AVFrame* frame = av_frame_alloc();
    AVFrame* filterFrame = av_frame_alloc();
    if (!frame || !filterFrame) {
        av_log(nullptr, AV_LOG_ERROR, "Could not allocate frame");
        exit(EXIT_FAILURE);
    }

    AVFormatContext* inputFormatContext = nullptr;
    const char* filename = argv[1];
    int ret;
    if ((ret = avformat_open_input(&inputFormatContext, filename, nullptr, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot open input file\n");
        return ret;
    }
    if ((ret = avformat_find_stream_info(inputFormatContext, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }
    AVCodec* decoder;
    // Select the video stream.
    ret = av_find_best_stream(inputFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot find a video stream in the input file\n");
        return ret;
    }
    int videoStreamIndex = ret;
    AVCodecContext* decodeContext;
    decodeContext = avcodec_alloc_context3(decoder);
    if (!decodeContext)
        return AVERROR(ENOMEM);

    avcodec_parameters_to_context(decodeContext, inputFormatContext->streams[videoStreamIndex]->codecpar);

    // Init the video decoder.
    if ((ret = avcodec_open2(decodeContext, decoder, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot open video decoder\n");
        return ret;
    }

    AVFilterGraph* filterGraph = avfilter_graph_alloc();
    AVFilterContext* bufferSrcContext;    // 进
    AVFilterContext* bufferSinkContext;   // 出
    const AVFilter* bufferSrc = avfilter_get_by_name("buffer");
    const AVFilter* bufferSink = avfilter_get_by_name("buffersink");
    AVFilterInOut* inputs = avfilter_inout_alloc();
    AVFilterInOut* outputs = avfilter_inout_alloc();
    if (!inputs || !outputs || !filterGraph) {
        av_log(nullptr, AV_LOG_ERROR, "Init filter failed\n");
        return EXIT_FAILURE;
    }

    AVRational timeBase = inputFormatContext->streams[videoStreamIndex]->time_base;
    enum AVPixelFormat pixelFormats[] = {AV_PIX_FMT_GRAY8, AV_PIX_FMT_NONE};

    // Buffer video source: the decoded frame from the decoder will be inserted here.
    char args[512];
    snprintf(args, sizeof(args),
             "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
             decodeContext->width,
             decodeContext->height,
             decodeContext->pix_fmt,
             timeBase.num,
             timeBase.den,
             decodeContext->sample_aspect_ratio.num,
             decodeContext->sample_aspect_ratio.den);

    ret = avfilter_graph_create_filter(&bufferSrcContext, bufferSrc, "in",
                                       args, nullptr, filterGraph);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot create buffer sink\n");
        return ret;
    }

    // Buffer video sink: to terminate the filter chain.
    ret = avfilter_graph_create_filter(&bufferSinkContext, bufferSink, "out",
                                       nullptr, nullptr, filterGraph);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot create buffer sink\n");
        return ret;
    }

    ret = av_opt_set_int_list((void*) bufferSinkContext, "pix_fmts", pixelFormats, AV_PIX_FMT_NONE,
                              AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot set output pixel format\n");
        return ret;
    }

    /**
     * Set the endpoints for the filter graph. The filter graph  will be linked to the graph described by
     * FILTERS_DESCR.
     *
     * The buffer source output must be connected to the input pad of the first filter described by
     * FILTERS_DESCR; since the first filter input label is not specified, it is set to "in" by default.
     */
    outputs->name = av_strdup("in");
    outputs->filter_ctx = bufferSrcContext;
    outputs->pad_idx = 0;
    outputs->next = nullptr;

    /**
     * The buffer sink input must be connect to the output pad of the last filter described by
     * FILTERS_DESCR; since the last filter output label is not specified, it is set to "out" by default.
     */
    inputs->name = av_strdup("out");
    inputs->filter_ctx = bufferSinkContext;
    inputs->pad_idx = 0;
    inputs->next = nullptr;

    if ((ret = avfilter_graph_parse_ptr(filterGraph, FILTER_DESCR,
                                        &inputs, &outputs, nullptr)) < 0) {
        return EXIT_FAILURE;
    }

    if ((ret = avfilter_graph_config(filterGraph, nullptr)) < 0) {
        return EXIT_FAILURE;
    }

    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);

    AVPacket packet;
    // Read all packets.
    while (true) {
        if ((ret = av_read_frame(inputFormatContext, &packet)) < 0)
            break;

        if (packet.stream_index == videoStreamIndex) {
            ret = avcodec_send_packet(decodeContext, &packet);
            if (ret < 0) {
                av_log(nullptr, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
                break;
            }

            while (ret >= 0) {
                ret = avcodec_receive_frame(decodeContext, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    av_log(nullptr, AV_LOG_ERROR, "Error while receiving a frame frame the decoder\n");
                    return ret;
                }
                frame->pts = frame->best_effort_timestamp;

                // Push the decoded frame into the filter graph.
                if (av_buffersrc_add_frame_flags(bufferSrcContext, frame, AV_BUFFERSRC_FLAG_KEEP_REF) < 0) {
                    av_log(nullptr, AV_LOG_ERROR, "Error while feeding the filter graph\n");
                    break;
                }

                // Pull filtered frames from the filter graph.
                while (true) {
                    ret = av_buffersink_get_frame(bufferSinkContext, filterFrame);
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                        break;
                    if (ret < 0)
                        return ret;

                    if (decodeContext->frame_number > 50 && decodeContext->frame_number < 55) {
                        char buf[1024];
                        snprintf(buf, sizeof(buf), "out/%s-%d.pgm", "frame", decodeContext->frame_number);
                        pgmSave(filterFrame, buf);
                    }

                    av_frame_unref(filterFrame);
                }
                av_frame_unref(frame);
            }
        }
        av_packet_unref(&packet);
    }

    avfilter_graph_free(&filterGraph);
    avcodec_free_context(&decodeContext);
    avformat_close_input(&inputFormatContext);
    av_frame_free(&frame);
    av_frame_free(&filterFrame);

    return EXIT_SUCCESS;
}
