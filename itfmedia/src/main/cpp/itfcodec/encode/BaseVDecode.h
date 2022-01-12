#ifndef ITFPLAYER_BASEVDECODE_H
#define ITFPLAYER_BASEVDECODE_H

#include <thread>
#include <mutex>

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

class BaseVDecode {
public:
    virtual void Source(char *url);

    void Resume();

    void Pause();

    void Stop();

    int State();


protected:
    void Looping();

    virtual int Encode() = 0;//纯虚解码 -2.unknown -1.end 0.continue

    //地址
    char *m_pUrl = nullptr;
    //当前请求操作
    volatile CodecRequest m_request;
    //当前状态
    volatile CodecState m_state;
    //自动开始
    volatile bool m_auto = true;
    //请求锁,保证状态变换顺序
    std::mutex m_pMutex;
    //循环锁,保证只存在单次循环
    std::mutex m_pLoopMutex;
    std::thread loopThread;
};

#endif //ITFPLAYER_BASEVDECODE_H
