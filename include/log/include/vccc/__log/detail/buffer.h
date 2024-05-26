# /*
#   Created by YongGyu Lee on 2021/04/05.
# */
#
# ifndef VCCC_LOG_DETAIL_BUFFER_H_
# define VCCC_LOG_DETAIL_BUFFER_H_
#
# include "boost/predef.h"
#
# define VCCC_LOG_EXPAND(x) x // defense for MSVC
#
# if BOOST_PLAT_ANDROID
#   include <android/log.h>
#
#   define VCCC_LOG_PRINTER(buf, tag, fmt, ...)  __android_log_print(buf, tag, fmt, __VA_ARGS__)
#
#   // https://developer.android.com/ndk/reference/group/logging
#   define VCCC_LOG_BUFFER_DEBUG    ANDROID_LOG_DEBUG
#   define VCCC_LOG_BUFFER_INFO     ANDROID_LOG_DEBUG
#   define VCCC_LOG_BUFFER_WARN     ANDROID_LOG_ERROR
#   define VCCC_LOG_BUFFER_ERROR    ANDROID_LOG_ERROR
#
# elif BOOST_OS_IOS
#   include "TargetConditionals.h"
#   include <syslog.h>
#
#   define VCCC_LOG_PRINTER(buf, tag, fmt, ...)  syslog(buf, "%s" fmt, tag, __VA_ARGS__)
#
#   // https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/syslog.3.html
#   define VCCC_LOG_BUFFER_DEBUG    LOG_DEBUG
#   define VCCC_LOG_BUFFER_INFO     LOG_DEBUG
#   define VCCC_LOG_BUFFER_WARN     LOG_ERR
#   define VCCC_LOG_BUFFER_ERROR    LOG_ERR
#
# else // for wasm, Windows, MacOS
#   include <cstdio>
#
#   define VCCC_LOG_PRINTER(buf, tag, fmt, ...)  VCCC_LOG_EXPAND(std::fprintf(buf, "%s" fmt "\n", tag, __VA_ARGS__))
#   define VCCC_LOG_BUFFER_DEBUG    stdout
#   define VCCC_LOG_BUFFER_INFO     stdout
#   define VCCC_LOG_BUFFER_WARN     stderr
#   define VCCC_LOG_BUFFER_ERROR    stderr
#
# endif
#
# endif //VCCC_LOG_DETAIL_BUFFER_H_
