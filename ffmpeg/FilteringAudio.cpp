//
// Created by wangrl on 2021/3/2.
//

/**
 * API example for audio decoding and filtering.
 */

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/opt.h>
}

static const char* FILTER_DESCR = "aresample=8000,aformat=sample_fmts=s16:channel_layouts=mono";
static const char* PLAYER = "ffplay -f s16le -ar 8000 -ac 1 -";

/**
 * 初始化音频过滤器。
 *
 * @param formatContext         [in] 输入文件的Context
 * @param filterGraph           [out] 过滤图
 * @param codecContext          [out] 解码的Context
 * @param bufferSrcContext      [out] 输入过滤器，连接过滤图
 * @param bufferSinkContext     [out] 输出过滤器，连接过滤图
 * @param audioStreamIndex      [in]  音频下标
 * @return                      小于0表示失败
 */
static int initFilters(AVFormatContext* formatContext,
                       AVFilterGraph** filterGraph,
                       AVCodecContext** codecContext,
                       AVFilterContext** bufferSrcContext,
                       AVFilterContext** bufferSinkContext,
                       int audioStreamIndex) {
    char args[512];
    int ret = 0;
    const AVFilter* abufferSrc = avfilter_get_by_name("abuffer");
    const AVFilter* aBufferSink = avfilter_get_by_name("abuffersink");
    AVFilterInOut* outputs = avfilter_inout_alloc();
    AVFilterInOut* inputs = avfilter_inout_alloc();
    static const enum AVSampleFormat outSampleFormats[] = {AV_SAMPLE_FMT_S16,
                                                           static_cast<const AVSampleFormat>(-1)};
    static const int64_t outChannelLayouts[] = {AV_CH_LAYOUT_MONO, -1};
    static const int outSampleRates[] = {8000, -1};
    const AVFilterLink* outLink;

    AVRational timeBase = formatContext->streams[audioStreamIndex]->time_base;

    *filterGraph = avfilter_graph_alloc();
    if (!outputs || !inputs || !*filterGraph) {
        ret = AVERROR(ENOMEM);
        goto end;
    }

    // Buffer audio source: the decoded frames from the decoder will be inserted here.
    if (!(*codecContext)->channel_layout)
        (*codecContext)->channel_layout = av_get_default_channel_layout((*codecContext)->channels);
    snprintf(args, sizeof(args),
             "time_base=%d/%d:sample_rate=%d:sample_fmt=%s:channel_layout=%ld",
             timeBase.num, timeBase.den, (*codecContext)->sample_rate,
             av_get_sample_fmt_name((*codecContext)->sample_fmt), (*codecContext)->channel_layout);
    ret = avfilter_graph_create_filter(bufferSrcContext, abufferSrc, "in",
                                       args, nullptr, *filterGraph);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot create audio buffer source\n");
        goto end;
    }

    // Buffer audio sink: to terminate the filter chain.
    ret = avfilter_graph_create_filter(bufferSinkContext, aBufferSink, "out",
                                       nullptr, nullptr, *filterGraph);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot create audio buffer sink\n");
        goto end;
    }

    ret = av_opt_set_int_list(*bufferSinkContext, "sample_fmts", outSampleFormats, -1,
                              AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot set output sample format\n");
        goto end;
    }
    ret = av_opt_set_int_list(*bufferSinkContext, "channel_layouts", outChannelLayouts, -1,
                              AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot set output channel layout\n");
        goto end;
    }

    ret = av_opt_set_int_list(*bufferSinkContext, "sample_rates", outSampleRates, -1,
                              AV_OPT_SEARCH_CHILDREN);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot set output sample rate\n");
        goto end;
    }

    // Set the endpoints for the filter graph. The filter graph will be
    // linked to the graph describe by FILTERS_DESCR.

    // The buffer source output must be connected to the input pad of
    // the first filter described by FILTERS_DESCR; since the first
    // filter input label is not specified, it is set to "in" by default.
    outputs->name = av_strdup("in");
    outputs->filter_ctx = *bufferSrcContext;
    outputs->pad_idx = 0;
    outputs->next = nullptr;

    // The buffer sink input must be connected to the output pad of
    // the last filter described by FILTERS_DESCR; since the last
    // filter output label is not specified, it is set to "out" by default.
    inputs->name = av_strdup("out");
    inputs->filter_ctx = *bufferSinkContext;
    inputs->pad_idx = 0;
    inputs->next = nullptr;

    if ((ret = avfilter_graph_parse_ptr(*filterGraph, FILTER_DESCR,
                                        &inputs, &outputs, nullptr)) < 0)
        goto end;
    if ((ret = avfilter_graph_config(*filterGraph, nullptr)) < 0)
        goto end;

    // Print summary of the sink buffer
    // Note: args buffer is reused to store channel layout string.
    outLink = (*bufferSinkContext)->inputs[0];
    av_get_channel_layout_string(args, sizeof(args), -1, outLink->channel_layout);
    av_log(nullptr, AV_LOG_INFO, "Output: sample_rate:%dHz fmt:%s ch_layout:%s\n",
           (int) outLink->sample_rate,
           (char*) av_x_if_null(av_get_sample_fmt_name(static_cast<AVSampleFormat>(outLink->format)), "?"),
           args);

    end:
    avfilter_inout_free(&inputs);
    avfilter_inout_free(&outputs);

    return ret;
}

int main(int argc, char** argv) {
    AVFormatContext* formatContext = nullptr;
    AVCodecContext* codecContext = nullptr;
    AVFilterContext* bufferSrcContext = nullptr;
    AVFilterContext* bufferSinkContext = nullptr;
    AVFilterGraph* filterGraph = nullptr;
    AVCodec* codec = nullptr;
    int audioStreamIndex = -1;
    int ret;
    AVPacket packet;
    AVFrame* frame = av_frame_alloc();
    AVFrame* filterFrame = av_frame_alloc();
    if (!frame || !filterFrame) {
        perror("Could not allocate frame");
        exit(EXIT_FAILURE);
    }

    if (argc != 3) {
        av_log(nullptr, AV_LOG_ERROR, "Usage: %s input_file | %s\n", argv[0], PLAYER);
        exit(EXIT_FAILURE);
    }
    const char* filename = argv[1];
    FILE* file = fopen(argv[2], "wb");

    if ((ret = avformat_open_input(&formatContext, filename, nullptr, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot open input file\n");
        return ret;
    }
    if ((ret = avformat_find_stream_info(formatContext, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }

    // Select the audio stream.
    ret = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);
    if (ret < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot find an audio stream in the input file\n");
        return ret;
    }
    audioStreamIndex = ret;

    // Create decoding context.
    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext)
        return AVERROR(ENOMEM);
    avcodec_parameters_to_context(codecContext, formatContext->streams[audioStreamIndex]->codecpar);

    // Init the audio decoder.
    if ((ret = avcodec_open2(codecContext, codec, nullptr)) < 0) {
        av_log(nullptr, AV_LOG_ERROR, "Cannot open audio decoder\n");
        return ret;
    }

    if ((ret = initFilters(formatContext, &filterGraph,
                           &codecContext, &bufferSrcContext, &bufferSinkContext,
                           audioStreamIndex)) < 0)
        goto end;

    while ((ret = av_read_frame(formatContext, &packet)) >= 0) {
        if (packet.stream_index == audioStreamIndex) {
            ret = avcodec_send_packet(codecContext, &packet);
            if (ret < 0) {
                av_log(nullptr, AV_LOG_ERROR, "Error while sending a packet to the decoder\n");
                break;
            }

            while (ret >= 0) {
                ret = avcodec_receive_frame(codecContext, frame);
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                    break;
                } else if (ret < 0) {
                    av_log(nullptr, AV_LOG_ERROR, "Error while receiving a frame from the decoder\n");
                    goto end;
                }
                if (ret >= 0) {
                    // Push the audio data from decoded frame into the filter graph.
                    if (av_buffersrc_add_frame_flags(bufferSrcContext, frame, AV_BUFFERSRC_FLAG_KEEP_REF) < 0) {
                        av_log(nullptr, AV_LOG_ERROR, "Error while feeding the audio filter graph\n");
                        break;
                    }

                    // Pull filtered audio from the filter graph.
                    while (true) {
                        ret = av_buffersink_get_frame(bufferSinkContext, filterFrame);
                        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                            break;
                        for (int i = 0; i < filterFrame->nb_samples; i++) {
                            fwrite(filterFrame->data[0] + sizeof(uint16_t) * i, 1, sizeof(uint16_t), file);
                        }
                        av_frame_unref(filterFrame);
                    }
                    av_frame_unref(frame);
                }
            }
        }
        av_packet_unref(&packet);
    }

    av_log(nullptr, AV_LOG_INFO, "Play command: ffplay -f s16le -ar 8000 -ac 1 %s\n", argv[2]);

    end:
    avfilter_graph_free(&filterGraph);
    avcodec_free_context(&codecContext);
    avformat_close_input(&formatContext);
    av_frame_free(&frame);
    av_frame_free(&filterFrame);

    if (ret < 0 && ret != AVERROR_EOF) {
        char str[AV_ERROR_MAX_STRING_SIZE];
        av_log(nullptr, AV_LOG_ERROR, "Error occurred: %s\n",
               av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, ret));
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
