#pragma once
#include <stdio.h>
#include <map>
#include <string>
#ifdef UNITY_ANDROID
#include <jni.h>
#else
#define JNIEXPORT
#endif

#ifndef WIN32
#include <android/log.h>
#ifdef DEBUG
#define LOGI(format, args...) __android_log_print(ANDROID_LOG_INFO, "UnityCpp", format, ##args)
#define LOGD(format, args...) __android_log_print(ANDROID_LOG_DEBUG, "UnityCpp", "%s:%d " format, __FILE__, __LINE__, ##args)
#define LOGE(format, args...) __android_log_print(ANDROID_LOG_ERROR, "UnityCpp", format, ##args)
#define LOG_INFO(format, args...) __android_log_print(ANDROID_LOG_INFO, "UnityCpp", format, ##args)
#define LOG_TRACE(format, args...) __android_log_print(ANDROID_LOG_DEBUG, "UnityCpp", format, ##args)
#define PLOGE(fmt, args...) LOGE("[%d: %s]" fmt, errno, strerror(errno), ##args)
#else
#define LOGD(format, args...)
#define LOGI(format, args...) __android_log_print(ANDROID_LOG_INFO, "UnityCpp", format, ##args)
#define LOGE(format, args...) __android_log_print(ANDROID_LOG_ERROR, "UnityCpp", format, ##args)
#define LOG_TRACE(format, args...)
#endif // ENABLE_LOG
#else
#define LOGD(format, args)
#define LOGI(format, args) 
#define LOGE(format, args) 
#define LOG_TRACE(format, args)
#endif

struct ZBlockIndex {
    uint64_t xx32;
    int32_t offset;
    int32_t size;
};

struct ZHeader {
    int32_t headerSize;
    int32_t nBlockNumber;
    std::map<int, ZBlockIndex> blockIndexMap;
};

struct ZBlock {
    uint64_t xx32;
    std::string line;
};

struct ZFile {
    FILE* fd{NULL};
    ZHeader header;
};

extern "C"
{
JNIEXPORT const ZFile* ZOpen(const char* path, const char* mode);
JNIEXPORT const int ZGet(ZFile* zfile, char* buffer, int size);
JNIEXPORT const int ZLoadHeader(ZFile* zfile);
JNIEXPORT const uint64_t ZGetBlockSize(ZFile* zfile, int key1, int key2);
JNIEXPORT const uint64_t ZGetBlock(ZFile* zfile, int key, char* buffer, int size);
JNIEXPORT const int ZClose(ZFile* zfile);
}
