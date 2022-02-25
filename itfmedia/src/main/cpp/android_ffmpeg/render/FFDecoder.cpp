#include "FFDecoder.h"

void FFDecoder::SourceVideo(char *pUrl) {
    m_pUrl = pUrl;
    m_avMediaType = AVMEDIA_TYPE_VIDEO;
    DecoderInitial();
}

void FFDecoder::SourceAudio(char *pUrl) {
    m_pUrl = pUrl;
    m_avMediaType = AVMEDIA_TYPE_AUDIO;
    DecoderInitial();
}

char *FFDecoder::Source() {
    return m_pUrl;
}

int FFDecoder::Decode() {
    //-1 stop, 0 continue, 1 complete
    int ret_;
    /* *
     * 1.读取编码数据包
     * */
    ret_ = av_read_frame(m_pAvFtmCtx, m_pAvPack);
    if (ret_ < 0) {
        return -1;
    }
    if (m_pAvPack->stream_index != m_avStreamIndex) {
        return -1;
    }
    /* *
     * 2.发送到解码队列
     * */
    ret_ = avcodec_send_packet(m_pAvCodecContext, m_pAvPack);
    if (ret_ != 0) {
        return -1;
    }
    /* *
     * 3.接收解码数据
     * */
    ret_ = avcodec_receive_frame(m_pAvCodecContext, m_pAvFrame);
    if (ret_ != 0) {
        return -1;
    }
    av_packet_unref(m_pAvPack);
    return 0;
}

AVFrame *FFDecoder::DecodeRet() {
    return m_pAvFrame;
}

int FFDecoder::MediaInfoDump(char *pUrl) {
    AVFormatContext *ftmCtx = avformat_alloc_context();
    int ret_;
    ret_ = avformat_open_input(&ftmCtx, pUrl, nullptr, nullptr);
    if (ret_ < 0) {
        LOGCAT("FFDecoder::MediaInfoDump -1");
        return -1;
    }
    ret_ = avformat_find_stream_info(ftmCtx, nullptr);
    if (ret_ < 0) {
        LOGCAT("FFDecoder::MediaInfoDump -2");
        return -2;
    }
    //AVFormatContext, 流索引(一般为0), url, 打印 输入/输出流(1:输出0:输入)
    //av_dump_format(m_pAvFtmCtx, 0, pUrl, 0);
    avformat_close_input(&ftmCtx);
    return 0;
}

void FFDecoder::Release() {
    delete m_pUrl;
    m_pUrl = nullptr;
    m_avMediaType = AVMEDIA_TYPE_UNKNOWN;
    if (m_pAvFtmCtx != nullptr) {
        avformat_free_context(m_pAvFtmCtx);
        m_pAvFtmCtx = nullptr;
    }
    m_avStreamIndex = 0;
    if (m_pAvCodecContext != nullptr) {
        avcodec_close(m_pAvCodecContext);
        avcodec_free_context(&m_pAvCodecContext);
        delete m_pAvCodecContext;
        m_pAvCodecContext = nullptr;
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
}

int FFDecoder::DecoderInitial() {
    if (m_pUrl == nullptr) {
        return -1;
    }
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
        LOGCAT("FFDecoder::DecoderInit avformat_open_input err: %s", av_err2str(ret_));
        return -2;
    }
    /* *
     * 3.获得音视频流信息
     * */
    ret_ = avformat_find_stream_info(m_pAvFtmCtx, nullptr);
    if (ret_ < 0) {
        LOGCAT("FFDecoder::DecoderInit avformat_find_stream_info err: %s", av_err2str(ret_));
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
        LOGCAT("FFDecoder::DecoderInit streamIndex find err");
        return -4;
    }
    /* *
     * 5.获取解码器
     * */
    AVCodecParameters *codecParameters = m_pAvFtmCtx->streams[m_avStreamIndex]->codecpar;
    m_pAvCodec = (AVCodec *) avcodec_find_decoder(codecParameters->codec_id);
    if (m_pAvCodec == nullptr) {
        LOGCAT("FFDecoder::DecoderInit avcodec_find_decoder err");
        return -5;
    }
    /* *
     * 6.创建解码器上下文
     * */
    m_pAvCodecContext = avcodec_alloc_context3(m_pAvCodec);
    ret_ = avcodec_parameters_to_context(m_pAvCodecContext, codecParameters);
    if (ret_ != 0) {
        LOGCAT("FFDecoder::DecoderInit avcodec_parameters_to_context err");
        return -6;
    }
    /* *
     * 7.打开解码器
     * */
    ret_ = avcodec_open2(m_pAvCodecContext, m_pAvCodec, nullptr);
    if (ret_ < 0) {
        LOGCAT("FFDecoder::DecoderInit avcodec_open2 err: %d", ret_);
        return -7;
    }
    /* *
     * 8.创建数据结构体
     * */
    m_pAvPack = av_packet_alloc();//创建 AVPacket 存放编码数据
    m_pAvFrame = av_frame_alloc();//创建 AVFrame 存放解码后的数据
    return 0;
}