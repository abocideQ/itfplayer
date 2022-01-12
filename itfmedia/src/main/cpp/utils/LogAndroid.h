#ifndef ITFPLAYER_LOGANDROID_H
#define ITFPLAYER_LOGANDROID_H

#include<android/log.h>

#define LOG_TAG "Android_LOG_"

#define LOGCAT(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define GL_ERROR_CHECK(...) LOGCAT(LOG_TAG + "GL_ERROR_CHECK %s error = %d, line = %d, ",  __FUNCTION__, glGetError(), __LINE__)

#endif //ITFPLAYER_LOGANDROID_H
