//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCCC_LOG_PC_HPP
#define VCCC_LOG_PC_HPP

#include <cstdio>

// TODO split debug macro
#define VCCC_LOG_DEBUG

#ifdef VCCC_LOG_DEBUG
  #define PRINT_WITH_NEWLINE(fmt, ...) std::printf(fmt "\n", __VA_ARGS__)
  #define FPRINT_WITH_NEWLINE(out, fmt, ...) std::fprintf(out, fmt "\n", __VA_ARGS__)

  #define EXPAND(x) x

  #define LOGV_IMPL(...) PRINT_WITH_NEWLINE(EXPAND(__VA_ARGS__))
  #define LOGD_IMPL(...) PRINT_WITH_NEWLINE(EXPAND(__VA_ARGS__))
  #define LOGI_IMPL(...) PRINT_WITH_NEWLINE(EXPAND(__VA_ARGS__))
  #define LOGW_IMPL(...) PRINT_WITH_NEWLINE(EXPAND(__VA_ARGS__))
  #define LOGE_IMPL(...) FPRINT_WITH_NEWLINE(stderr, EXPAND(__VA_ARGS__))
  #define LOGF_IMPL(...) PRINT_WITH_NEWLINE(EXPAND(__VA_ARGS__))
  #define LOGS_IMPL(...) PRINT_WITH_NEWLINE(EXPAND(__VA_ARGS__))
#else
  #define LOGV_IMPL(...)
  #define LOGD_IMPL(...)
  #define LOGI_IMPL(...)
  #define LOGW_IMPL(...)
  #define LOGE_IMPL(...)
  #define LOGF_IMPL(...)
  #define LOGS_IMPL(...)
# endif

# endif //VCCC_LOG_PC_HPP
