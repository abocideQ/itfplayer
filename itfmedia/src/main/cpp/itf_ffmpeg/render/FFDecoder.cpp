#include "FFDecoder.h"

extern "C" {
void FFDecoder::SourceVideo(char *pUrl) {
    Release();
    std::lock_guard<std::mutex> lock_request_state(m_stateMutex);
    FFDecoderCore::FFOpen(pUrl, AVMEDIA_TYPE_VIDEO);
    m_pThread = new std::thread(&FFDecoder::Looping, this);
}

void FFDecoder::SourceAudio(char *pUrl) {
    Release();
    std::lock_guard<std::mutex> lock_request_state(m_stateMutex);
    FFDecoderCore::FFOpen(pUrl, AVMEDIA_TYPE_AUDIO);
    m_pThread = new std::thread(&FFDecoder::Looping, this);
}

void FFDecoder::Resume() {
    std::lock_guard<std::mutex> lock_request_state(m_stateMutex);
    if (m_pThread == nullptr) {
        return;
    }
    if (m_state == STATE_STOP || m_state == STATE_UNKNOWN) {
        return;
    }
    m_request = REQUEST_RESUME;
}

void FFDecoder::Pause() {
    std::lock_guard<std::mutex> lock_request_state(m_stateMutex);
    if (m_pThread == nullptr) {
        return;
    }
    if (m_state == STATE_STOP || m_state == STATE_UNKNOWN) {
        return;
    }
    m_request = REQUEST_PAUSE;
}

void FFDecoder::Stop() {
    std::unique_lock<std::mutex> lock_request_state(m_stateMutex);
    if (m_pThread == nullptr) {
        return;
    }
    m_request = REQUEST_STOP;
    lock_request_state.unlock();
    if (m_pThread->joinable()) {
        m_pThread->join();
    }
    delete m_pThread;
    m_pThread = nullptr;
}

void FFDecoder::Release() {
    std::unique_lock<std::mutex> lock_request_state(m_stateMutex);
    if (m_pThread == nullptr) {
        return;
    }
    m_request = REQUEST_STOP;
    m_pDecodeVideoListener = nullptr;
    m_pDecodeAudioListener = nullptr;
    m_pDecodeListenerCtx = nullptr;
    lock_request_state.unlock();
    if (m_pThread->joinable()) {
        m_pThread->join();
    }
    delete m_pThread;
    m_pThread = nullptr;
    FFDecoderCore::FFClose();
}

void FFDecoder::Looping() {
    std::lock_guard<std::mutex> lock_loop(m_loopMutex);
    m_state = STATE_READY;
    //decode looping
    for (;;) {
        std::unique_lock<std::mutex> lock_request_state(m_stateMutex);
        //??????
        if (m_request == REQUEST_RESUME) {//??????
            m_request = REQUEST_NULL;
            m_state = STATE_RESUME;
            lock_request_state.unlock();
            continue;
        } else if (m_request == REQUEST_PAUSE) {//??????
            m_request = REQUEST_NULL;
            m_state = STATE_PAUSE;
            lock_request_state.unlock();
            continue;
        } else if (m_request == REQUEST_STOP) {//??????
            m_request = REQUEST_NULL;
            m_state = STATE_STOP;
            lock_request_state.unlock();
            return;
        }
        //??????
        if (m_state == STATE_READY) {//??????
            if (m_auto) {
                m_state = STATE_RESUME;
            } else {
                m_state = STATE_PAUSE;
            }
            lock_request_state.unlock();
            continue;
        } else if (m_state == STATE_RESUME) {//??????
            int ret_ = FFDecoderCore::FFDecode();
            if (ret_ == -1) {//????????????
                m_state = STATE_ERR_DECODE;
            } else if (ret_ == 1) {//????????????
                m_state = STATE_COMPLETE;
            }
            lock_request_state.unlock();
            continue;
        } else if (m_state == STATE_PAUSE) {//??????
            lock_request_state.unlock();
            continue;
        } else if (m_state == STATE_STOP) {//??????
            return;
        } else if (m_state == STATE_COMPLETE) {//??????
            return;
        } else if (m_state == STATE_UNKNOWN) {//??????
            return;
        } else if (m_state == STATE_ERR_INIT || m_state == STATE_ERR_NET ||
                   m_state == STATE_ERR_DECODE || m_state == STATE_ERR_OTHER) {//??????
            return;
        }
    }
}

void FFDecoder::FFDecodeVideoRet(int w, int h, uint8_t *data[8]) {
    if (m_pDecodeListenerCtx != nullptr && m_pDecodeVideoListener != nullptr) {
        m_pDecodeVideoListener(m_pDecodeListenerCtx, w, h, data);
    }
}

void FFDecoder::FFDecodeAudioRet(int size, uint8_t *data[8]) {
    if (m_pDecodeListenerCtx != nullptr && m_pDecodeAudioListener != nullptr) {
        m_pDecodeAudioListener(m_pDecodeListenerCtx, size, data);
    }
}

void FFDecoder::SetDecodeVideoListener(void *ctx, DecodeVideoListener listener) {
    m_pDecodeListenerCtx = ctx;
    m_pDecodeVideoListener = listener;
}

void FFDecoder::SetDecodeAudioListener(void *ctx, DecodeAudioListener listener) {
    m_pDecodeListenerCtx = ctx;
    m_pDecodeAudioListener = listener;
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
    } else if (m_state == STATE_ERR_INIT) {
        return -1;
    } else if (m_state == STATE_ERR_NET) {
        return -2;
    } else if (m_state == STATE_ERR_DECODE) {
        return -3;
    } else if (m_state == STATE_ERR_OTHER) {
        return -4;
    } else {
        return -10000;
    }
}

int FFDecoder::Seek2Percent(float percent) {
    return FFDecoderCore::FFPosition(percent);
}

int64_t FFDecoder::Position() {
    return FFDecoderCore::FFPosition();
}

int64_t FFDecoder::Duration() {
    return FFDecoderCore::FFDuration();
}
}