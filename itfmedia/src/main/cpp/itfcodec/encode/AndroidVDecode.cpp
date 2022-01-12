#include "AndroidVDecode.h"

AndroidVDecode::AndroidVDecode() {
    m_decoder = new FFVDecode();
}

void AndroidVDecode::Source(char *url) {
    BaseVDecode::Source(url);
    m_decoder->Source(url);
}

int AndroidVDecode::Encode() {
    int ret = m_decoder->MediaInfoDump();
    if (ret < 0) {
        return -2;
    } else {
        return -2;
    }
}