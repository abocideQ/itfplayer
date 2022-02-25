#ifndef ITFPLAYER_ITFDECODER_H
#define ITFPLAYER_ITFDECODER_H

#include "FFDecoder.h"

//当前解码状态
enum CodecState {
    STATE_UNKNOWN,
    STATE_READY,
    STATE_RESUME,
    STATE_PAUSE,
    STATE_STOP,
    STATE_COMPLETE
};

//当前解码请求
enum CodecRequest {
    REQUEST_NULL,
    REQUEST_RESUME,
    REQUEST_PAUSE,
    REQUEST_STOP
};

class ItfDecoder {
public:
    void Source(char *pUrl);

    void Resume();

    void Pause();

    void Stop();

    void Release();

    int State();

protected:
    //解码器运行时
    void Looping();

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

    //ffDecoder
    FFDecoder *m_pFFDecoder;
};

#endif //ITFPLAYER_ITFDECODER_H
