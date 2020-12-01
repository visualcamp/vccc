//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCPP_LOG_ANDROID_HPP
#define VCPP_LOG_ANDROID_HPP

#include <android/log.h>

#ifndef LOG_TAG
  #define LOG_TAG "LOG_TAG_ANDROID"
#endif

#ifdef NDEBUG
  #define VCPP_LOG_DEBUG

  #define LOGV_IMPL(...)
  #define LOGD_IMPL(...)
  #define LOGI_IMPL(...)
  #define LOGW_IMPL(...)
  #define LOGE_IMPL(...)
  #define LOGF_IMPL(...)
  #define LOGS_IMPL(...)
#else
  #define LOGV_IMPL(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
  #define LOGD_IMPL(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
  #define LOGI_IMPL(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
  #define LOGW_IMPL(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
  #define LOGE_IMPL(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
  #define LOGF_IMPL(...) __android_log_print(ANDROID_FATAL_ERROR, LOG_TAG, __VA_ARGS__)
  #define LOGS_IMPL(...) __android_log_print(ANDROID_SILENT_ERROR, LOG_TAG, __VA_ARGS__)
#endif

#endif //VCPP_LOG_ANDROID_HPP
