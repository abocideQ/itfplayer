#ifndef ITFPLAYER_FFDECODERCORE_H
#define ITFPLAYER_FFDECODERCORE_H

#include "utils/header_common.h"
#include "utils/header_ffmpeg.h"

static long long SystemCurrentMilli() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((long long) (tv.tv_sec)) * 1000 + tv.tv_usec / 1000;
}

class FFDecoderCore {
public:
    //媒体信息
//    int FFInfoDump(char *pUrl);

protected:
    //初始化
    int FFOpen(char *pUrl, AVMediaType avMediaType);

    //解码
    int FFDecode();

    //视频回调
    virtual void FFDecodeVideoRet(int w, int h, uint8_t *data[8]) = 0;

    //音频回调
    virtual void FFDecodeAudioRet(int size, uint8_t *data[8]) = 0;

    int64_t FFDuration();

    int64_t FFPosition();

    int FFPosition(float seek2);

    //销毁
    void FFClose();

private:
    //视频处理
    void SwsScale(AVFrame *frame);

    //音频处理
    void SwrConvert(AVFrame *frame);

    //同步
    void Synchronization(AVFrame *frame);

    void PixCharge(AVPixelFormat f, int w, int h, int lines[8], uint8_t *data[8]);

    //媒体地址
    char *m_pUrl = nullptr;
    //媒体类型
    AVMediaType m_avMediaType = AVMEDIA_TYPE_UNKNOWN;
    //AVFormatContext 上下文
    AVFormatContext *m_pAvFtmCtx = nullptr;
    //音视频流索引
    int m_avStreamIndex = 0;
    //视频从0s开始播放的时间戳(当前时间戳 - 当前流时间)
    int64_t m_avStartMilli = -1;
    //当前播放进度
    int64_t m_avCurrentStamp = -1;
    //seek_percent
    volatile float m_avSeekPercent = 0;

    //解码器
    AVCodec *m_pAvCodec = nullptr;
    //解码器上下文
    AVCodecContext *m_pAvCodecCtx = nullptr;
    //编码数据结构体
    AVPacket *m_pAvPack = nullptr;
    //解码数据结构体
    AVFrame *m_pAvFrame = nullptr;
    //视频转换
    SwsContext *m_pSwsCtx = nullptr;
    AVFrame *m_pSwsFrame = nullptr;
    //音频采样
    SwrContext *m_pSwrCtx = nullptr;
    uint8_t *m_pSwrFrame = nullptr;
    uint8_t m_pSwrFrame_nb_samples = 0;
    uint8_t m_pSwrFrame_size = 0;
    //同步锁
    std::mutex m_synMutex;
    //pixel
    uint8_t *pixel[8];
};

#endif //ITFPLAYER_FFDECODERCORE_H
