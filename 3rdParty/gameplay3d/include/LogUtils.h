#ifndef _LOG_UTILS_H_
#define _LOG_UTILS_H_

#ifdef __ANDROID__

#include <android/log.h>

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

//로그 정의
#define  LOG_TAG    "GameEngineCore"
#define  LOG_API_TAG    "GameEngineCoreJNI"

#if 0 //def NDEBUG
#define  LOGD(...)
#define  LOGI(...)
#define  LOGW(...)
#define  LOGE(...)
#define  LOGF(...)
#define  LOGS(...)
#define  LOGV(...)
#else
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#define  LOGS(...)  __android_log_print(ANDROID_LOG_SILENT,LOG_TAG,__VA_ARGS__)
#define  LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define  LOGAPI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_API_TAG,__VA_ARGS__)
#endif

// LOGX Unicode 버전 // 일단 아래와 같이 만들어서 사용하면 좋을것 같습니다. (S = String, I = int, F = float, 등등...)
void LOGDWS(const char *fmt, const wchar_t *arg0);
void LOGVWS(const char *fmt, const wchar_t *arg0);
void LOGDWC(const char *fmt, const wchar_t arg0);
void LOGDWCS(const char *fmt, const wchar_t arg0, const wchar_t* arg1);
void LOGDWICI(const char *fmt, const int arg0, const wchar_t arg1, const int arg2);
//void LOGDWIS(const char *fmt, int arg0, const wchar_t *arg1);
//void LOGDWIIS(const char *fmt, int arg0, int arg1, const wchar_t *arg2);
void LOGDWSS(const char *fmt, const wchar_t *arg0, const wchar_t *arg1);
void LOGDWSSS(const char *fmt, const wchar_t *arg0, const wchar_t *arg1, const wchar_t *arg2);

void LOGVWS(const char *fmt, const wchar_t *arg0);

/** prefix와 함께 unicode 문자열을 로그로 출력 */
void logUnicode(const char *prefix, const wchar_t *unicode);
/** prefix와 함께 unicode 문자를 로그로 출력 */
void logUnicodeChar(const char *prefix, const wchar_t wc);

void convertUnicodeToUtf8(const wchar_t *unicode, char *utf8);

#ifdef __cplusplus
}
#endif

#else // __ANDROID__
#define  LOGD(...)
#define  LOGI(...)
#define  LOGW(...)
#define  LOGE(...)
#define  LOGF(...)
#define  LOGS(...)
#define  LOGV(...)
#endif

#endif // _LOG_UTILS_H_
