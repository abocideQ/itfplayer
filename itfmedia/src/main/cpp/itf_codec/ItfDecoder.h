#ifndef ITFPLAYER_ITFDECODER_H
#define ITFPLAYER_ITFDECODER_H

#include "FFDecoder.h"

class ItfDecoder {
public:
    void Source(char *pUrl);

    void Resume();

    void Pause();

    void Stop();

    void Release();

    int State();

    static void DecodeVideoListener(void *ctx, int w, int h, uint8_t *data[8]);

    static void DecodeAudioListener(void *ctx, int size, uint8_t *data[8]);

private:
    //VideoDecoder
    FFDecoder *m_pVideoDecoder;
    //AudioDecoder
    FFDecoder *m_pAudioDecoder;
};

#endif //ITFPLAYER_ITFDECODER_H
