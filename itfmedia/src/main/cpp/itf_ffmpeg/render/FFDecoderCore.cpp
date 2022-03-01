#include "FFDecoderCore.h"

extern "C" {
int FFDecoderCore::FFOpen(char *pUrl, AVMediaType avMediaType) {
    m_pUrl = pUrl;
    m_avMediaType = avMediaType;
    /* *
     * 1.申请context
     * */
    if (m_pAvFtmCtx == nullptr) {
        m_pAvFtmCtx = avformat_alloc_context();
    }
    int ret_;
    /* *
     * 2.打开输入 解封装
     * AVFormatContext + url + 指定的输入格式(一般为NULL，由ffmpeg自行解析) + 附加参数(一般为NULL)
     * */
    ret_ = avformat_open_input(&m_pAvFtmCtx, m_pUrl, nullptr, nullptr);
    if (ret_ < 0) {
        LOGCAT("FFDecoderCore::DecoderInit avformat_open_input err: %s", av_err2str(ret_));
        return -2;
    }
    /* *
     * 3.获得音视频流信息
     * */
    ret_ = avformat_find_stream_info(m_pAvFtmCtx, nullptr);
    if (ret_ < 0) {
        LOGCAT("FFDecoderCore::DecoderInit avformat_find_stream_info err: %s", av_err2str(ret_));
        return -3;
    }
    /* *
     * 4.获得音视频流索引
     * nb_streams: number of AVFormatContext.streams
     * codecpar: AVCodecParameters
     * codec_type: type of codec
     * */
    int i = 0;
    for (; i < m_pAvFtmCtx->nb_streams; i++) {
        if (m_pAvFtmCtx->streams[i]->codecpar->codec_type == m_avMediaType) {
            m_avStreamIndex = i;
            break;
        }
    }
    if (m_avStreamIndex < 0) {
        LOGCAT("FFDecoderCore::DecoderInit streamIndex find err");
        return -4;
    }
    /* *
     * 5.获取解码器
     * */
    AVCodecParameters *codecParameters = m_pAvFtmCtx->streams[m_avStreamIndex]->codecpar;
    m_pAvCodec = (AVCodec *) avcodec_find_decoder(codecParameters->codec_id);
    if (m_pAvCodec == nullptr) {
        LOGCAT("FFDecoderCore::DecoderInit avcodec_find_decoder err");
        return -5;
    }
    /* *
     * 6.创建解码器上下文
     * */
    m_pAvCodecCtx = avcodec_alloc_context3(m_pAvCodec);
    ret_ = avcodec_parameters_to_context(m_pAvCodecCtx, codecParameters);
    if (ret_ != 0) {
        LOGCAT("FFDecoderCore::DecoderInit avcodec_parameters_to_context err");
        return -6;
    }
    /* *
     * 7.打开解码器
     * */
    ret_ = avcodec_open2(m_pAvCodecCtx, m_pAvCodec, nullptr);
    if (ret_ < 0) {
        LOGCAT("FFDecoderCore::DecoderInit avcodec_open2 err: %d", ret_);
        return -7;
    }
    /* *
     * 8.创建数据结构体
     * */
    m_pAvPack = av_packet_alloc();//创建 AVPacket 存放编码数据
    m_pAvFrame = av_frame_alloc();//创建 AVFrame 存放解码后的数据
    return 0;
}

int FFDecoderCore::FFDecode() {
    //-1 stop, 0 continue, 1 complete
    int ret_;
    /* *
     * 1.读取编码数据包
     * AVPack 对应 1个NALU 对应 1个或多个frames
     * PTS:渲染时间戳
     * DTS:解码时间戳
     * */
    ret_ = av_read_frame(m_pAvFtmCtx, m_pAvPack);
    if (ret_ < 0 || m_pAvPack->stream_index != m_avStreamIndex) {
        av_packet_unref(m_pAvPack);
        LOGCAT("FFDecoderCore::Decode av_read_frame err -1");
        return -1;
    }
    /* *
     * 2.发送到解码队列
     * */
    ret_ = avcodec_send_packet(m_pAvCodecCtx, m_pAvPack);
    if (ret_ == AVERROR_EOF) {//end of file 解码结束
        av_packet_unref(m_pAvPack);
        LOGCAT("FFDecoderCore::Decode avcodec_send_packet end");
        return 1;
    }
    /* *
     * 3.接收解码数据
     * */
    int nb_frame = 0;
    while (avcodec_receive_frame(m_pAvCodecCtx, m_pAvFrame) == 0) {
        {//音频/视频数据处理
            if (m_avMediaType == AVMEDIA_TYPE_VIDEO) {
                SwsScale(m_pAvFrame);
            } else if (m_avMediaType == AVMEDIA_TYPE_AUDIO) {
                SwrConvert(m_pAvFrame);
            }
        }
        {//时钟同步
            Synchronization();
        }
        nb_frame++;
    }
    if (nb_frame > 0) {
        av_packet_unref(m_pAvPack);
    }
    return 0;
}

void FFDecoderCore::FFClose() {
    delete m_pUrl;
    m_pUrl = nullptr;
    m_avMediaType = AVMEDIA_TYPE_UNKNOWN;
    //codec
    if (m_pAvFtmCtx != nullptr) {
        avformat_free_context(m_pAvFtmCtx);
        m_pAvFtmCtx = nullptr;
    }
    m_avStreamIndex = 0;
    if (m_pAvCodecCtx != nullptr) {
        avcodec_close(m_pAvCodecCtx);
        avcodec_free_context(&m_pAvCodecCtx);
        delete m_pAvCodecCtx;
        m_pAvCodecCtx = nullptr;
        delete m_pAvCodec;
        m_pAvCodec = nullptr;
    }
    if (m_pAvPack != nullptr) {
        av_packet_free(&m_pAvPack);
        delete m_pAvPack;
        m_pAvPack = nullptr;
    }
    if (m_pAvFrame != nullptr) {
        av_frame_free(&m_pAvFrame);
        delete m_pAvFrame;
        m_pAvFrame = nullptr;
    }
    //sws
    if (m_pSwsCtx != nullptr) {
        sws_freeContext(m_pSwsCtx);
        m_pSwsCtx = nullptr;
    }
    pixel[0] = nullptr;
    pixel[1] = nullptr;
    pixel[2] = nullptr;
    *pixel = nullptr;
    //swr
    if (m_pSwrFrame != nullptr) {
        swr_free(m_pSwrCtx);
        m_pSwrCtx = nullptr;
        free(m_pSwrFrame);
        m_pSwrFrameSize = 0;
    }
}

int FFDecoderCore::FFInfoDump(char *pUrl) {
    AVFormatContext *ftmCtx = avformat_alloc_context();
    int ret_;
    ret_ = avformat_open_input(&ftmCtx, pUrl, nullptr, nullptr);
    if (ret_ < 0) {
        LOGCAT("FFDecoderCore::MediaInfoDump -1");
        return -1;
    }
    ret_ = avformat_find_stream_info(ftmCtx, nullptr);
    if (ret_ < 0) {
        LOGCAT("FFDecoderCore::MediaInfoDump -2");
        return -2;
    }
    //AVFormatContext, 流索引(一般为0), url, 打印 输入/输出流(1:输出0:输入)
    //av_dump_format(m_pAvFtmCtx, 0, pUrl, 0);
    avformat_close_input(&ftmCtx);
    return 0;
}

void FFDecoderCore::SwsScale(AVFrame *frame) {
    int width = m_pAvCodecCtx->width;
    int height = m_pAvCodecCtx->height;
    if (m_pSwsCtx == nullptr) {
        int align = 1;
        /* *
         * align: 内存对齐参数, 对齐是为了兼容硬件编解码
         * 图片原数据: 3*10像素图片(rbg8bit模式 每像素3字节) -> 图片每行占9字节=3像素*3字节
         * 若align=2: 每行应为2*5=10字节 -> ffmpeg在数据存储时每行尾部填充1字节
         * 若align=1: 按实际的大小进行存储，不对齐,若解码时已经内存对齐则此处align=1无效
         * linesize: 行字节数
         * */
        //转换用frame初始化
        m_pSwsFrame = av_frame_alloc();
        //转换用frame内存分配
        int frameSize = av_image_get_buffer_size(AV_PIX_FMT_NV21, width, height, align);
        uint8_t *frameBuffer = (uint8_t *) av_malloc(frameSize);
        av_image_fill_arrays(m_pSwsFrame->data, m_pSwsFrame->linesize, frameBuffer,
                             AV_PIX_FMT_NV21, width, height, align);
        //===========
        //swsCtx初始化
        m_pSwsCtx = sws_getContext(width, height, m_pAvCodecCtx->pix_fmt,
                                   width, height, AV_PIX_FMT_NV21,
                                   SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);

    }
    //图像格式转换->yuv420p/nv21/nv12-> data struct
    int pixelScale = 0;
    AVPixelFormat pixelFormat;
    if (m_pAvCodecCtx->pix_fmt == AV_PIX_FMT_YUV420P ||
        m_pAvCodecCtx->pix_fmt == AV_PIX_FMT_YUVJ420P) {
        pixelFormat = AV_PIX_FMT_YUV420P;
        if (frame->data[0] && frame->data[1] && !frame->data[2] &&
            frame->linesize[0] == frame->linesize[1] && frame->linesize[2] == 0) {
            //h264 mediacodec decoder is NV12/NV21 兼容某些设备可能出现的格式不匹配问题
            pixelFormat = AV_PIX_FMT_NV21;
        }
    } else if (m_pAvCodecCtx->pix_fmt != AV_PIX_FMT_NV21) {
        pixelFormat = AV_PIX_FMT_NV21;
    } else if (m_pAvCodecCtx->pix_fmt != AV_PIX_FMT_NV12) {
        pixelFormat = AV_PIX_FMT_NV12;
    } else {
        int ret_ = sws_scale(m_pSwsCtx,
                             m_pAvFrame->data, m_pAvFrame->linesize, 0, m_pAvFrame->height,
                             m_pSwsFrame->data, m_pSwsFrame->linesize);
        pixelFormat = AV_PIX_FMT_NV21;
        pixelScale = 1;
    }
    //数据处理(内存对齐/....etc.)
    if (pixelScale) {
        PixCharge(pixelFormat, width, height, m_pSwsFrame->linesize, m_pSwsFrame->data);
    } else {
        PixCharge(pixelFormat, width, height, m_pAvFrame->linesize, m_pAvFrame->data);
    }
    //视频回调
    FFDecodeVideoRet(width, height, pixel);
}

void FFDecoderCore::SwrConvert(AVFrame *frame) {
    if (m_pSwrCtx == nullptr) {
        int align = 1;
        //swrCtx初始化
        m_pSwrCtx = swr_alloc();
        //入/出通道数
        av_opt_set_int(m_pSwrCtx, "in_channel_layout", m_pAvCodecCtx->channel_layout, 0);
        av_opt_set_int(m_pSwrCtx, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);//双声道立体声
        //采样率
        av_opt_set_int(m_pSwrCtx, "in_sample_rate", m_pAvCodecCtx->sample_rate, 0);
        av_opt_set_int(m_pSwrCtx, "out_sample_rate", 44100, 0);//CD标准采样率
        //格式 16位packed格式 1个缓存区: C1C2C1C2, 结尾带P为planar格式: 每个通道1个缓存区
        av_opt_set_int(m_pSwrCtx, "in_sample_fmt", m_pAvCodecCtx->sample_fmt, 0);
        av_opt_set_int(m_pSwrCtx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
        swr_init(m_pSwrCtx);
        //=================

        //转换用frame样本数计算 -> 将a个样本, 由c采样率 转换为 b采样率, nb_samples = a*b/c
        int64_t delay = swr_get_delay(m_pSwrCtx, frame->nb_samples);           //上次没转换完的样本数
        m_pSwrFrame_nb_samples = (int) av_rescale_rnd(1024 + delay,            //a每次转换的样本数
                                                      44100,                   //b输出采样率
                                                      frame->sample_rate,      //c输入采样率
                                                      AV_ROUND_UP);            //向上取整
        //转换用frame内存分配
        int frameSize = av_samples_get_buffer_size(NULL,                   //lineSize
                                                   m_pAvCodecCtx->channels,//channels
                                                   m_pSwrFrame_nb_samples, //samples
                                                   AV_SAMPLE_FMT_S16,      //fmt
                                                   align);                 //align
        m_pSwrFrame = (uint8_t *) malloc(frameSize);
    }
    int ret_ = swr_convert(m_pSwrCtx,
                           &m_pSwrFrame, m_pSwrFrame_nb_samples,
                           (const uint8_t **) frame->data, frame->nb_samples);
    //音频回调
    FFDecodeAudioRet(0, pixel);
}

void FFDecoderCore::Synchronization() {
    std::lock_guard<std::mutex> lock_syn(m_synMutex);
}

void FFDecoderCore::PixCharge(AVPixelFormat f, int w, int h, int *lines, uint8_t *data[8]) {
    uint8_t *y = nullptr;
    uint8_t *u = nullptr;
    uint8_t *v = nullptr;
//    int yuvSize = w * h * 3 / 2;
    if (f == AV_PIX_FMT_YUV420P) {
        y = new uint8_t[w * h];
        u = new uint8_t[w * h / 4];
        v = new uint8_t[w * h / 4];
        if (w != lines[0]) {
            int i = 0;
            for (; i < h; i++) {
                memcpy(y + w * i, data[0] + lines[0] * i, w);
            }
            i = 0;
            for (; i < h / 2; i++) {
                memcpy(u + w * i / 2, data[1] + lines[1] * i, w / 2);
                memcpy(v + w * i / 2, data[2] + lines[2] * i, w / 2);
            }
        } else {
            memcpy(y, data[0], lines[0]);
            memcpy(u, data[1], lines[1] / 4);
            memcpy(v, data[2], lines[2] / 4);
        }
    } else if (f == AV_PIX_FMT_NV21 || f == AV_PIX_FMT_NV12) {
        y = new uint8_t[w * h];
        u = new uint8_t[w * h / 2];
        if (w != lines[0]) {
            int i = 0;
            for (; i < h; i++) {
                memcpy(y + w * i, data[0] + lines[0] * i, w);
            }
            i = 0;
            for (; i < h / 2; i++) {
                memcpy(u + w * i, data[1] + lines[1] * i, w);
            }
        } else {
            memcpy(y, data[0], lines[0]);
            memcpy(u, data[1], lines[1] / 2);
        }
    }
    pixel[0] = y;
    pixel[1] = u;
    pixel[2] = v;
}
}