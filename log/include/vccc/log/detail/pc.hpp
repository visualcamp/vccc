//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCCC_LOG_PC_HPP
#define VCCC_LOG_PC_HPP

#include <cstdio>

// TODO split debug macro
#define VCCC_LOG_DEBUG

#ifdef VCCC_LOG_DEBUG
#define EXPAND(x) x

  #define PRINT_WITH_NEWLINE(fmt, ...) EXPAND(std::printf(fmt "\n", __VA_ARGS__))
  #define FPRINT_WITH_NEWLINE(out, fmt, ...) EXPAND(std::fprintf(out, fmt "\n", __VA_ARGS__))

  #define LOGV_IMPL(...) EXPAND(PRINT_WITH_NEWLINE(__VA_ARGS__))
  #define LOGD_IMPL(...) EXPAND(PRINT_WITH_NEWLINE(__VA_ARGS__))
  #define LOGI_IMPL(...) EXPAND(PRINT_WITH_NEWLINE(__VA_ARGS__))
  #define LOGW_IMPL(...) EXPAND(PRINT_WITH_NEWLINE(__VA_ARGS__))
  #define LOGE_IMPL(...) EXPAND(FPRINT_WITH_NEWLINE(stderr, __VA_ARGS__))
  #define LOGF_IMPL(...) EXPAND(PRINT_WITH_NEWLINE(__VA_ARGS__))
  #define LOGS_IMPL(...) EXPAND(PRINT_WITH_NEWLINE(__VA_ARGS__))
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
