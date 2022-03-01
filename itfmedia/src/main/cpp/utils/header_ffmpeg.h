#ifndef ITFPLAYER_HEADER_FFMPEG_H
#define ITFPLAYER_HEADER_FFMPEG_H

#define PLATFORM_ANDROID
#undef PLATFORM_IOS

#ifdef PLATFORM_ANDROID

extern "C" {
#include "libavformat/avformat.h"//av_format
#include "libavcodec/avcodec.h"//codec

#include "libswscale/swscale.h"//视频转换
#include "libavutil/imgutils.h"//图像工具

#include "libswresample/swresample.h"//音频重采样
#include <libavutil/opt.h>//音频工具
};

#elif PLATFORM_IOS

//???????

#endif

#endif //ITFPLAYER_HEADER_FFMPEG_H
