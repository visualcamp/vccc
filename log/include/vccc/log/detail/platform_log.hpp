//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCCC_LOG_PLATFORM_LOG
#define VCCC_LOG_PLATFORM_LOG

/**
 *      If you add another header for that platform, it must
 *      define VCCC_LOG_DEBUG when really writes out
 */

#ifdef __ANDROID__
#include "vccc/log/detail/android.hpp"
#elif __APPLE__ && TARGET_OS_IPHONE
#include "vccc/log/detail/ios.hpp"
#else
#include "vccc/log/detail/pc.hpp"
#endif

#endif //VCCC_LOG_PLATFORM_LOG
