#ifndef ITFPLAYER_FFDECODERCORE_H
#define ITFPLAYER_FFDECODERCORE_H

#include "utils/header_common.h"
#include "utils/header_ffmpeg.h"

class FFDecoderCore {
public:
    //媒体信息
    int FFInfoDump(char *pUrl);

protected:
    //初始化
    int FFOpen(char *pUrl, AVMediaType avMediaType);

    //解码
    int FFDecoder();

    //解码回调
    virtual void FFDecoderRet() = 0;

    //销毁
    void FFClose();

private:
    //视频处理
    void SwsScale();

    //音频处理
    void SwrConvert();

    //同步
    void Synchronization();

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
    //视频转换/音频采样
    SwsContext *m_pSwsCtx = nullptr;
    //同步锁
    std::mutex m_synMutex;
};

#endif //ITFPLAYER_FFDECODERCORE_H
