#ifndef _SYS_LOG_
#define _SYS_LOG_

#include <libavutil/log.h>

#if defined __cplusplus
extern "C"
{
#endif // __cplusplus

#define LOGD(format, ...) av_log(NULL, AV_LOG_DEBUG, format, ##__VA_ARGS__);
#define LOGV(format, ...) av_log(NULL, AV_LOG_VERBOSE, format, ##__VA_ARGS__);
#define LOGI(format, ...) av_log(NULL, AV_LOG_INFO, format, ##__VA_ARGS__);
#define LOGW(format, ...) av_log(NULL, AV_LOG_WARNING, format, ##__VA_ARGS__);
#define LOGE(format, ...) av_log(NULL, AV_LOG_ERROR, format, ##__VA_ARGS__);

#ifdef __ANDROID_API__
#include <android/log.h>

#define ALOG(level, TAG, ...)    ((void)__android_log_vprint(level, TAG, __VA_ARGS__))

#define SYS_LOG_TAG "ffmpeg"

static void syslog_print(void *ptr, int level, const char *fmt, va_list vl)
{
    switch(level) {
    case AV_LOG_DEBUG:
        ALOG(ANDROID_LOG_VERBOSE, SYS_LOG_TAG, fmt, vl);
        break;
    case AV_LOG_VERBOSE:
        ALOG(ANDROID_LOG_DEBUG, SYS_LOG_TAG, fmt, vl);
        break;
    case AV_LOG_INFO:
        ALOG(ANDROID_LOG_INFO, SYS_LOG_TAG, fmt, vl);
        break;
    case AV_LOG_WARNING:
        ALOG(ANDROID_LOG_WARN, SYS_LOG_TAG, fmt, vl);
        break;
    case AV_LOG_ERROR:
        ALOG(ANDROID_LOG_ERROR, SYS_LOG_TAG, fmt, vl);
        break;
    }
}

#endif // __ANDROID_API__

static void syslog_init()
{
    av_log_set_callback(syslog_print);
    av_log_set_level(AV_LOG_VERBOSE);
}

#if defined __cplusplus
}
#endif // __cplusplus

#endif
