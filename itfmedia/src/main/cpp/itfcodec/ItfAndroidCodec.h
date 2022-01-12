#ifndef ITFPLAYER_ITFANDROIDCODEC_H
#define ITFPLAYER_ITFANDROIDCODEC_H

#include "AndroidVDecode.h"

class ItfAndroidCodec {
public:
    void Source(char *url);

    static ItfAndroidCodec *instance();

protected:
    AndroidVDecode *m_vDecode;

private:

    static ItfAndroidCodec *m_itfCodec;
};


#endif //ITFPLAYER_ITFANDROIDCODEC_H
