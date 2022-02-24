#include "FFDecoder.h"

void FFDecoder::Source(char *pUrl, AVMediaType avMediaType) {
    m_pUrl = pUrl;
    m_avMediaType = avMediaType;
}

int FFDecoder::DecoderInit() {
    if (m_pUrl == nullptr) {
        return -1;
    }
    /* *
     * 1.申请context
     * */
    if (m_pFtmCtx == nullptr) {
        m_pFtmCtx = avformat_alloc_context();
    }
    int ret_ = 0;
    /* *
     * 2.打开输入 解封装
     * AVFormatContext + url + 指定的输入格式(一般为NULL，由ffmpeg自行解析) + 附加参数(一般为NULL)
     * */
    ret_ = avformat_open_input(&m_pFtmCtx, m_pUrl, nullptr, nullptr);
    if (ret_ < 0) {
        LOGCAT("FFDecoder::DecoderInit avformat_open_input err: %s", av_err2str(ret_));
        return -2;
    }
    /* *
     * 3.获得音视频流信息
     * */
    ret_ = avformat_find_stream_info(m_pFtmCtx, nullptr);
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
    int stream_index = 0;
    int i = 0;
    for (; i < m_pFtmCtx->nb_streams; i++) {
        if (m_pFtmCtx->streams[i]->codecpar->codec_type == m_avMediaType) {
            m_streamIndex = i;
            break;
        }
    }
    if (m_streamIndex < 0) {
        LOGCAT("FFDecoder::DecoderInit streamIndex find err");
        return -4;
    }
    /* *
     * 5.获取解码器
     * */
    AVCodecParameters *codecParameters = m_pFtmCtx->streams[m_streamIndex]->codecpar;
    m_pAVCodec = (AVCodec *) avcodec_find_decoder(codecParameters->codec_id);
    if (m_pAVCodec == nullptr) {
        LOGCAT("FFDecoder::DecoderInit avcodec_find_decoder err");
        return -5;
    }
    /* *
     * 6.创建解码器上下文
     * */
    m_pAVCodecContext = avcodec_alloc_context3(m_pAVCodec);
    ret_ = avcodec_parameters_to_context(m_pAVCodecContext, codecParameters);
    if (ret_ != 0) {
        LOGCAT("FFDecoder::DecoderInit avcodec_parameters_to_context err");
        return -6;
    }
    /* *
     * 7.打开解码器
     * */
    ret_ = avcodec_open2(m_pAVCodecContext, m_pAVCodec, nullptr);
    if (ret_ < 0) {
        LOGCAT("FFDecoder::DecoderInit avcodec_open2 err: %d", ret_);
        return -7;
    }
    /* *
     * 8.创建数据结构体
     * */
    m_pAVPack = av_packet_alloc();//创建 AVPacket 存放编码数据
    m_pAVFrame = av_frame_alloc();//创建 AVFrame 存放解码后的数据
    return 0;
}

int FFDecoder::MediaInfoDump() {
    if (m_pUrl == nullptr) {
        return -1;
    }
    /* *
     * 1.申请context
     * */
    if (m_pFtmCtx == nullptr) {
        m_pFtmCtx = avformat_alloc_context();
    }
    int ret_ = 0;
    /* *
     * 2.打开输入 解封装
     * AVFormatContext + url + 指定的输入格式(一般为NULL，由ffmpeg自行解析) + 附加参数(一般为NULL)
     * */
    ret_ = avformat_open_input(&m_pFtmCtx, m_pUrl, NULL, NULL);
    if (ret_ < 0) {
        LOGCAT("FFDecoder::MediaInfoDump avformat_open_input err: \n %s", av_err2str(ret_));
        return -2;
    }
    /* *
     * 3.获得音视频流信息
     * */
    ret_ = avformat_find_stream_info(m_pFtmCtx, 0);
    if (ret_ < 0) {
        LOGCAT("FFDecoder::MediaInfoDump avformat_find_stream_info err: \n %s", av_err2str(ret_));
        return -3;
    }
    //av_dump_format(m_pFtmCtx, 0, m_pUrl, 0); //AVFormatContext, 流索引(一般为0), url, 打印 输入/输出流(1:输出0:输入)
    /* *
     * 4.关闭输入
     * */
    avformat_close_input(&m_pFtmCtx);
    return 0;
}

void FFDecoder::Release() {
    delete m_pUrl;
    m_pUrl = nullptr;
    m_avMediaType = AVMEDIA_TYPE_UNKNOWN;
    if (m_pFtmCtx != nullptr) {
        avformat_free_context(m_pFtmCtx);
        m_pFtmCtx = nullptr;
    }
    m_streamIndex = 0;
    if (m_pAVCodecContext != nullptr) {
        avcodec_close(m_pAVCodecContext);
        avcodec_free_context(&m_pAVCodecContext);
        delete m_pAVCodecContext;
        m_pAVCodecContext = nullptr;
        delete m_pAVCodec;
        m_pAVCodec = nullptr;
    }
    if (m_pAVPack != nullptr) {
        av_packet_free(&m_pAVPack);
        delete m_pAVPack;
        m_pAVPack = nullptr;
    }
    if (m_pAVFrame != nullptr) {
        av_frame_free(&m_pAVFrame);
        delete m_pAVFrame;
        m_pAVFrame = nullptr;
    }
}