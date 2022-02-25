#ifndef ITFPLAYER_FFDECODER_H
#define ITFPLAYER_FFDECODER_H

#include "utils/header_common.h"
#include "utils/header_ffmpeg.h"

class FFDecoder {
public:
    //资源
    void SourceVideo(char *pUrl);

    void SourceAudio(char *pUrl);

    //返回链接
    char *Source();

    //解码
    int Decode();

    //解码后数据
    AVFrame *DecodeRet();

    //销毁
    void Release();

    //打印媒体信息
    int MediaInfoDump(char *pUrl);

protected:
    //解码器初始化
    int DecoderInitial();

private:
    //媒体地址
    char *m_pUrl = nullptr;
    //媒体类型
    AVMediaType m_avMediaType = AVMEDIA_TYPE_UNKNOWN;
    //AVFormatContext 上下文
    AVFormatContext *m_pAvFtmCtx = nullptr;
    //音视频流索引
    int m_avStreamIndex = 0;
    //解码器
    AVCodec *m_pAvCodec = nullptr;
    //解码器上下文
    AVCodecContext *m_pAvCodecContext = nullptr;
    //编码数据结构体
    AVPacket *m_pAvPack = nullptr;
    //解码数据结构体
    AVFrame *m_pAvFrame = nullptr;
};

#endif //ITFPLAYER_FFDECODER_H
