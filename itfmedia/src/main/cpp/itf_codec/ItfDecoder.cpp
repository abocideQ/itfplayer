#include "ItfDecoder.h"

extern "C" {
void ItfDecoder::Source(char *pUrl) {
    if (m_pVideoDecoder != nullptr) {
        m_pVideoDecoder->Release();
        delete m_pVideoDecoder;
        m_pVideoDecoder = nullptr;
    }
    if (m_pAudioDecoder != nullptr) {
        m_pAudioDecoder->Release();
        delete m_pAudioDecoder;
        m_pAudioDecoder = nullptr;
    }
    if (m_pVideoDecoder == nullptr) {
        m_pVideoDecoder = new FFDecoder();
        m_pVideoDecoder->SourceVideo(pUrl);
        m_pVideoDecoder->SetDecodeVideoListener(this, DecodeVideoListener);
    }
    if (m_pAudioDecoder == nullptr) {
        m_pAudioDecoder = new FFDecoder();
        m_pAudioDecoder->SourceAudio(pUrl);
        m_pAudioDecoder->SetDecodeAudioListener(this, DecodeAudioListener);
    }
}

void ItfDecoder::Resume() {
    if (m_pVideoDecoder == nullptr) {
        m_pVideoDecoder->Resume();
    }
    if (m_pAudioDecoder == nullptr) {
        m_pAudioDecoder->Resume();
    }
}

void ItfDecoder::Pause() {
    if (m_pVideoDecoder == nullptr) {
        m_pVideoDecoder->Pause();
    }
    if (m_pAudioDecoder == nullptr) {
        m_pAudioDecoder->Pause();
    }
}

void ItfDecoder::Stop() {
    if (m_pVideoDecoder == nullptr) {
        m_pVideoDecoder->Stop();
    }
    if (m_pAudioDecoder == nullptr) {
        m_pAudioDecoder->Stop();
    }
}

void ItfDecoder::Release() {
    if (m_pVideoDecoder != nullptr) {
        m_pVideoDecoder->Release();
        delete m_pVideoDecoder;
        m_pVideoDecoder = nullptr;
    }
    if (m_pAudioDecoder != nullptr) {
        m_pAudioDecoder->Release();
        delete m_pAudioDecoder;
        m_pAudioDecoder = nullptr;
    }
}

int ItfDecoder::State() {
    if (m_pVideoDecoder != nullptr) {
        return m_pVideoDecoder->State();
    }
    if (m_pAudioDecoder != nullptr) {
        return m_pAudioDecoder->State();
    }
    return -9999;
}

void ItfDecoder::DecodeVideoListener(void *ctx, int w, int h, uint8_t *data[8]) {
//    ItfDecoder *pItf = static_cast<ItfDecoder *>(ctx);
}

void ItfDecoder::DecodeAudioListener(void *ctx, int size, uint8_t *data[8]) {
//    ItfDecoder *pItf = static_cast<ItfDecoder *>(ctx);
}
}
