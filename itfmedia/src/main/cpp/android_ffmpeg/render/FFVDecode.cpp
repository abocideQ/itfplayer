#include "FFVDecode.h"

FFVDecode::FFVDecode() {
    if (ftm_ctx == nullptr) {
        ftm_ctx = avformat_alloc_context();
    }
}

void FFVDecode::Source(char *url) {
    m_pUrl = url;
}

int FFVDecode::MediaInfoDump() {
    if (m_pUrl == nullptr) {
        return -1;
    }
    //AVFormatContext + url + 指定的输入格式(一般为NULL，由ffmpeg自行解析) + 附加参数(一般为NULL)
    int ret = avformat_open_input(&ftm_ctx, m_pUrl, NULL, NULL);
    if (ret < 0) {
        LOGCAT("FFVDecode::MediaInfoDump avformat_open_input error: \n %s", av_err2str(ret));
        return -1;
    }
    /*//为AVFormatContext + 流索引(一般为0) + url + 打印 输入/输出流(1:输出0:输入)
    av_dump_format(ftm_ctx, 0, m_pUrl, 0);*/
    ret = avformat_find_stream_info(ftm_ctx, 0);
    if (ret < 0) {
        LOGCAT("FFVDecode::MediaInfoDump avformat_find_stream_info error: \n %s", av_err2str(ret));
        return -1;
    }
    //关闭输入
    avformat_close_input(&ftm_ctx);
    return 0;
}

void FFVDecode::Destroy() {
    m_pUrl = nullptr;
    avformat_free_context(ftm_ctx);
}