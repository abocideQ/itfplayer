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

private:
    //VideoDecoder
    FFDecoder *m_pVideoDecoder;
    //AudioDecoder
    FFDecoder *m_pAudioDecoder;
};

#endif //ITFPLAYER_ITFDECODER_H
