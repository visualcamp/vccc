//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCPP_LOG_PLATFORM_LOG
#define VCPP_LOG_PLATFORM_LOG

/**
 *      If you add another header for that platform, it must
 *      define VCPP_LOG_DEBUG when really writes out
 */

#ifdef __ANDROID__
#include "vcpp/log/detail/android.hpp"
#elif __APPLE__ && TARGET_OS_IPHONE
#include "vcpp/log/detail/ios.hpp"
#else
#include "vcpp/log/detail/pc.hpp"
#endif

#endif //VCPP_LOG_PLATFORM_LOG
