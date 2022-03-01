#ifndef ITFPLAYER_FFDECODER_H
#define ITFPLAYER_FFDECODER_H

#include "FFDecoderCore.h"

enum CodecState {
    STATE_UNKNOWN,
    STATE_READY,
    STATE_RESUME,
    STATE_PAUSE,
    STATE_STOP,
    STATE_COMPLETE,
    STATE_ERR_INIT,
    STATE_ERR_NET,
    STATE_ERR_DECODE,
    STATE_ERR_OTHER
};

enum CodecRequest {
    REQUEST_NULL,
    REQUEST_RESUME,
    REQUEST_PAUSE,
    REQUEST_STOP
};

typedef void (*DecodeVideoListener)(void *ctx, int w, int h, uint8_t **);

typedef void (*DecodeAudioListener)(void *ctx, int size, uint8_t **);

class FFDecoder : public FFDecoderCore {
public:
    void SourceVideo(char *pUrl);

    void SourceAudio(char *pUrl);

    void Resume();

    void Pause();

    void Stop();

    void Release();

    int State();

    void SetDecodeVideoListener(void *ctx, DecodeVideoListener listener);

    void SetDecodeAudioListener(void *ctx, DecodeAudioListener listener);

protected:
    //解码器运行时
    void Looping();

    void FFDecodeVideoRet(int w, int h, uint8_t *data[8]);

    void FFDecodeAudioRet(int size, uint8_t *data[8]);

    void *m_pDecodeListenerCtx = nullptr;

    DecodeVideoListener m_pDecodeVideoListener = nullptr;

    DecodeAudioListener m_pDecodeAudioListener = nullptr;

private:
    //自动开始
    volatile bool m_auto = true;
    //当前请求操作
    volatile CodecRequest m_request;
    //当前状态
    volatile CodecState m_state;
    //请求锁,保证状态变换顺序
    std::mutex m_stateMutex;
    //循环锁,保证只存在单次循环
    std::mutex m_loopMutex;
    //线程
    std::thread *m_pThread;
};


#endif //ITFPLAYER_FFDECODER_H
