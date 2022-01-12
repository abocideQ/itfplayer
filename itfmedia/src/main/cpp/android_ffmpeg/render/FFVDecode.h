#ifndef ITFPLAYER_FFVDECODE_H
#define ITFPLAYER_FFVDECODE_H

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
};

#include <thread>
#include "utils/LogAndroid.h"

class FFVDecode {
public:
    FFVDecode();

    //初始化
    void Source(char *url);

    //媒体信息
    int MediaInfoDump();

    //媒体信息
    int MediaInfoDump();

    //销毁
    void Destroy();

private:
    //媒体地址
    char *m_pUrl = nullptr;

    //AVFormatContext 上下文
    AVFormatContext *ftm_ctx = nullptr;
};

#endif //ITFPLAYER_FFVDECODE_H
