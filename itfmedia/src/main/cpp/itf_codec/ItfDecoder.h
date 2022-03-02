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

    int SeekTo(float percent);

    int64_t Position();

    int64_t Duration();

private:
    static void DecodeVideoListener(void *ctx, int w, int h, uint8_t *data[8]);

    static void DecodeAudioListener(void *ctx, int size, uint8_t *data[8]);

    //VideoDecoder
    FFDecoder *m_pVideoDecoder;
    //AudioDecoder
    FFDecoder *m_pAudioDecoder;
};

#endif //ITFPLAYER_ITFDECODER_H
