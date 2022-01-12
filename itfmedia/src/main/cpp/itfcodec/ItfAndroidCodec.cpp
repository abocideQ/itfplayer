#include "ItfAndroidCodec.h"

void ItfAndroidCodec::Source(char *url) {
    m_vDecode = new AndroidVDecode();
    m_vDecode->Init(url);
}

ItfAndroidCodec *ItfAndroidCodec::m_itfCodec = nullptr;
ItfAndroidCodec *ItfAndroidCodec::instance() {
    if (m_itfCodec == nullptr) {
        m_itfCodec = new ItfAndroidCodec();
    }
    return m_itfCodec;
}
