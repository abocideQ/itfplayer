#include "ItfVDecoder.h"

extern "C" {
void ItfVDecoder::Source(char *pUrl) {
//    if (m_pUrl != nullptr) {
//        Stop();
//    }
//    std::lock_guard<std::mutex> lock_request_state(m_mutex);
//    m_pUrl = url;
//    std::thread(&ItfVDecoder::Looping, this);
}

void ItfVDecoder::Resume() {
    std::lock_guard<std::mutex> request_state_lock(m_mutex);
    if (m_state == STATE_STOP || m_state == STATE_UNKNOWN) {
        return;
    }
    m_request = REQUEST_RESUME;
}

void ItfVDecoder::Pause() {
    std::lock_guard<std::mutex> lock_request_state(m_mutex);
    m_request = REQUEST_PAUSE;
}

void ItfVDecoder::Stop() {
    std::lock_guard<std::mutex> lock_request_state(m_mutex);
    m_request = REQUEST_STOP;
}

void ItfVDecoder::Looping() {
    std::unique_lock<std::mutex> lock_loop(m_loopMutex);
    m_state = STATE_READY;
    //codec looping
    for (;;) {
        std::unique_lock<std::mutex> lock_request_state(m_mutex);
        if (m_request == REQUEST_RESUME) {//请求继续
            m_request = REQUEST_NULL;
            m_state = STATE_RESUME;
            lock_request_state.unlock();
        } else if (m_request == REQUEST_PAUSE) {//请求暂停
            m_request = REQUEST_NULL;
            m_state = STATE_PAUSE;
            lock_request_state.unlock();
            continue;
        } else if (m_request == REQUEST_STOP) {//请求停止
            m_request = REQUEST_NULL;
            m_state = STATE_STOP;
            lock_request_state.unlock();
            return;
        } else if (m_state == STATE_PAUSE) {//暂停状态
            m_request = REQUEST_NULL;
            lock_request_state.unlock();
            continue;
        } else if (m_state == STATE_READY) {//就绪状态
            if (m_auto) {
                m_request = REQUEST_NULL;
                m_state = STATE_RESUME;
            }
            lock_request_state.unlock();
            continue;
        }
        int encode = Encode();
        if (encode == -2) {//解码停止
            m_state = STATE_STOP;
            return;
        } else if (encode == -1) {//解码结束
            m_state = STATE_COMPLETE;
            return;
        } else if (encode == 0) {//解码成功
            continue;
        }
    }
    lock_loop.unlock();
}

int ItfVDecoder::State() {
    if (m_state == STATE_UNKNOWN) {
        return -1;
    } else if (m_state == STATE_READY) {
        return 0;
    } else if (m_state == STATE_RESUME) {
        return 1;
    } else if (m_state == STATE_PAUSE) {
        return 2;
    } else if (m_state == STATE_STOP) {
        return 3;
    } else {
        return -2;
    }
}
}

int ItfVDecoder::Encode() {
    return 0;
}
