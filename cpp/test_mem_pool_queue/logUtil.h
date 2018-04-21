//
// Created by mj on 16-2-19.
//

#ifndef MVBOX_NEW_LOGUTIL_H
#define MVBOX_NEW_LOGUTIL_H

#ifdef __ANDROID__
#include <android/log.h>

//#define TAG "VideoRecorder"
//#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)
//#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__)
//#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__)
//#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__)
//#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__)

#include "const.h"
#include "QueueManager.h"

namespace vvav {
    extern bool g_enableFileLog;
    extern int g_logLevel;

}//namespace vvav

#define TAG "VideoRecorder"
#define LOGD(LOGFMT, ...) if(g_logLevel <= LVL_DEBUG){ \
    __android_log_print(ANDROID_LOG_DEBUG,TAG ,LOGFMT, ##__VA_ARGS__); \
    } \
    if(g_enableFileLog){ \
        char msg[8192]; \
        memset(msg, 0, 8192); \
        sprintf(msg, LOGFMT, ##__VA_ARGS__); \
        QueueManager::getInstance()->getLogQueue()->push(LVL_DEBUG, TAG, msg);\
}

#define LOGI(LOGFMT, ...) if(g_logLevel <= LVL_INFO){ \
    __android_log_print(ANDROID_LOG_INFO,TAG ,LOGFMT, ##__VA_ARGS__); \
    } \
    if(g_enableFileLog){ \
        char msg[8192]; \
        memset(msg, 0, 8192); \
        sprintf(msg, LOGFMT, ##__VA_ARGS__); \
        QueueManager::getInstance()->getLogQueue()->push(LVL_INFO, TAG, msg);\
}

#define LOGW(LOGFMT, ...) if(g_logLevel <= LVL_WARN){ \
    __android_log_print(ANDROID_LOG_WARN,TAG ,LOGFMT, ##__VA_ARGS__); \
    } \
    if(g_enableFileLog){ \
        char msg[8192]; \
        memset(msg, 0, 8192); \
        sprintf(msg, LOGFMT, ##__VA_ARGS__); \
        QueueManager::getInstance()->getLogQueue()->push(LVL_WARN, TAG, msg);\
}

#define LOGE(LOGFMT, ...) if(g_logLevel <= LVL_ERROR){ \
    __android_log_print(ANDROID_LOG_ERROR,TAG ,LOGFMT, ##__VA_ARGS__); \
    } \
    if(g_enableFileLog){ \
        char msg[8192]; \
        memset(msg, 0, 8192); \
        sprintf(msg, LOGFMT, ##__VA_ARGS__); \
        QueueManager::getInstance()->getLogQueue()->push(LVL_ERROR, TAG, msg);\
}

#define LOGF(LOGFMT, ...) if(g_logLevel <= LVL_FATAL){ \
    __android_log_print(ANDROID_LOG_FATAL,TAG ,LOGFMT, ##__VA_ARGS__); \
    } \
    if(g_enableFileLog){ \
        char msg[8192]; \
        memset(msg, 0, 8192); \
        sprintf(msg, LOGFMT, ##__VA_ARGS__); \
        QueueManager::getInstance()->getLogQueue()->push(LVL_FATAL, TAG, msg);\
}

#else

#include <stdio.h>

#define LOGD(LOGFMT, ...) printf(LOGFMT, ##__VA_ARGS__);printf("\n")
#define LOGI(LOGFMT, ...) printf(LOGFMT, ##__VA_ARGS__);printf("\n")
#define LOGW(LOGFMT, ...) printf(LOGFMT, ##__VA_ARGS__);printf("\n")
#define LOGE(LOGFMT, ...) printf(LOGFMT, ##__VA_ARGS__);printf("\n")



#endif

#endif //MVBOX_NEW_LOGUTIL_H
