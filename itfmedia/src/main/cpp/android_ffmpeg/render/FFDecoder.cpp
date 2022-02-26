#include "FFDecoder.h"

extern "C" {
void FFDecoder::SourceVideo(char *pUrl) {
    Stop();
    std::lock_guard<std::mutex> lock_request_state(m_stateMutex);
    FFDecoderCore::FFOpen(pUrl, AVMEDIA_TYPE_VIDEO);
    std::thread(&FFDecoder::Looping, this);
}

void FFDecoder::SourceAudio(char *pUrl) {
    Stop();
    std::lock_guard<std::mutex> lock_request_state(m_stateMutex);
    FFDecoderCore::FFOpen(pUrl, AVMEDIA_TYPE_AUDIO);
    std::thread(&FFDecoder::Looping, this);
}

void FFDecoder::Resume() {
    std::lock_guard<std::mutex> lock_request_state(m_stateMutex);
    if (m_state == STATE_STOP || m_state == STATE_UNKNOWN) {
        return;
    }
    m_request = REQUEST_RESUME;
}

void FFDecoder::Pause() {
    std::lock_guard<std::mutex> lock_request_state(m_stateMutex);
    m_request = REQUEST_PAUSE;
}

void FFDecoder::Stop() {
    std::lock_guard<std::mutex> lock_request_state(m_stateMutex);
    m_request = REQUEST_STOP;
}

void FFDecoder::Release() {
    std::lock_guard<std::mutex> lock_request_state(m_stateMutex);
    m_request = REQUEST_STOP;
    FFDecoderCore::FFClose();
}

void FFDecoder::Looping() {
    std::lock_guard<std::mutex> lock_loop(m_loopMutex);
    m_state = STATE_READY;
    //decode looping
    for (;;) {
        std::unique_lock<std::mutex> lock_request_state(m_stateMutex);
        //请求
        if (m_request == REQUEST_RESUME) {//继续
            m_request = REQUEST_NULL;
            m_state = STATE_RESUME;
            lock_request_state.unlock();
            continue;
        } else if (m_request == REQUEST_PAUSE) {//暂停
            m_request = REQUEST_NULL;
            m_state = STATE_PAUSE;
            lock_request_state.unlock();
            continue;
        } else if (m_request == REQUEST_STOP) {//停止
            m_request = REQUEST_NULL;
            m_state = STATE_STOP;
            lock_request_state.unlock();
            return;
        }
        //状态
        if (m_state == STATE_READY) {//就绪
            if (m_auto) {
                m_state = STATE_RESUME;
            } else {
                m_state = STATE_PAUSE;
            }
            lock_request_state.unlock();
            continue;
        } else if (m_state == STATE_RESUME) {//继续
            int ret_ = FFDecoderCore::FFDecoder();
            if (ret_ == -1) {//解码停止
                m_state = STATE_STOP;
            } else if (ret_ == 1) {//解码完成
                m_state = STATE_COMPLETE;
            }
            lock_request_state.unlock();
            continue;
        } else if (m_state == STATE_PAUSE) {//暂停
            lock_request_state.unlock();
            continue;
        } else if (m_state == STATE_STOP) {//停止
            return;
        } else if (m_state == STATE_COMPLETE) {//完成
            return;
        } else if (m_state == STATE_UNKNOWN) {//未知
            return;
        }
    }
}

int FFDecoder::State() {
    if (m_state == STATE_READY) {
        return 0;
    } else if (m_state == STATE_RESUME) {
        return 1;
    } else if (m_state == STATE_PAUSE) {
        return 2;
    } else if (m_state == STATE_STOP) {
        return 3;
    } else if (m_state == STATE_COMPLETE) {
        return 100;
    } else if (m_state == STATE_UNKNOWN) {
        return -100;
    }
}
}