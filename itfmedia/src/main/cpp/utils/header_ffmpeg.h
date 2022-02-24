#ifndef ITFPLAYER_HEADER_FFMPEG_H
#define ITFPLAYER_HEADER_FFMPEG_H

#define PLATFORM_ANDROID
#undef PLATFORM_IOS

#ifdef PLATFORM_ANDROID

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
};

#elif PLATFORM_IOS

//???????

#endif

#endif //ITFPLAYER_HEADER_FFMPEG_H
