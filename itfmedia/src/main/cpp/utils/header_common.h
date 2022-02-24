#ifndef ITFPLAYER_HEADER_COMMON_H
#define ITFPLAYER_HEADER_COMMON_H

#define PLATFORM_ANDROID
#undef PLATFORM_IOS

#ifdef PLATFORM_ANDROID

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include "utils/LOGANDROID.h"

#elif PLATFORM_IOS

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include "utils/ILOG.h"

#endif

#endif //ITFPLAYER_HEADER_COMMON_H
