#ifndef ITFPLAYER_FFDECODER_H
#define ITFPLAYER_FFDECODER_H

#include "utils/header_common.h"
#include "utils/header_ffmpeg.h"

class FFDecoder {
public:
    //初始化
    void Source(char *pUrl, AVMediaType avMediaType);

    //媒体信息
    int MediaInfoDump();

    //销毁
    void Release();

protected:
    //初始化解码器
    int DecoderInit();

private:
    //媒体地址
    char *m_pUrl = nullptr;
    //媒体类型
    AVMediaType m_avMediaType = AVMEDIA_TYPE_UNKNOWN;
    //AVFormatContext 上下文
    AVFormatContext *m_pFtmCtx = nullptr;
    //音视频流索引
    int m_streamIndex = 0;
    //解码器
    AVCodec *m_pAVCodec = nullptr;
    //解码器上下文
    AVCodecContext *m_pAVCodecContext = nullptr;
    //编码数据包
    AVPacket *m_pAVPack = nullptr;
    //解码数据包
    AVFrame *m_pAVFrame = nullptr;
};

#endif //ITFPLAYER_FFDECODER_H
