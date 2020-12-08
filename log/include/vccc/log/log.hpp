//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCCC_LOG_LOG_HPP
#define VCCC_LOG_LOG_HPP

#include "vccc/log/detail/platform_log.hpp"
#include "vccc/log/logger.hpp"

namespace vc {


class Log{
 public:

  /** forward arguments to predefined macros
   *  v for LOGV, d for LOGD, i for LOGI, w for LOGW, e for LOGE
   *
   * supports both printf-like format and ostringstream& operator <<
   * ex)
   *      Log.d("Hello, world")                   -> Hello, world
   *      Log.d("First:", 3, "Second:", 3.14)     -> First: 3 Second: 3.14
   *
   *      Log.d("Hello, %s", "world")             -> Hello, world
   *      Log.d("First: %d Second: %f", 3, 3.14)  -> First: 3 Second: 3.14
   *
   * Mixing both won't work
   * ex)
   *      Log.d("First: %d", 3, "Second:", 3.14)  -> First: 3 (wrong output; works exactly the same as printf)
   *      Log.d("First:", 3, "Second: %f", 3.14)  -> First: 3 Second: %f 3.14 (wrong output; works exactly the same as std::cout)
   *
   * printf-like format only works when
   *      1. first argument is (const) char* type
   *      2. following arguments are scalar (pointer, integer, floating-points, enum)
   *      and of course the output will be wrong if the wrong format is provided
   * ex)
   *      Log.d("Hello, %s", std::string("world"))    -> Hello, %s world
   *      Log.d(std::string("Hello, %s"), "world")    -> Hello, %s world
   *      Log.d("Int: %d", 3.14)                      -> Int: -492425104 (wrong output example)
   *
   * if ostringstream& operator << is overloaded for user-defined types, it can be printed
   * ex)
   *      Log.d("string:", std::string("wow!"))   -> string: wow!
   *      Log.d("Point:", cv::Point2i(3,4))       -> Point: [3, 4]
   *
   * if using << format, blank will be inserted between all elements
   * ex)
   *      Log.d("ABCDEF")         -> ABCDEF
   *      Log.d("ABC", "DEF")     -> ABC DEF
   *      Log.d("AB", "CD", 12)   -> AB CD 12
   *
   */

  static inline Log& getInstance() {
    static Log log_;
    return log_;
  }

  template<typename ...Args> void v(Args&&... args) const;
  template<typename ...Args> void d(Args&&... args) const;
  template<typename ...Args> void i(Args&&... args) const;
  template<typename ...Args> void w(Args&&... args) const;
  template<typename ...Args> void e(Args&&... args) const;

 private:
  Log() = default;

};

//static Log Log;


template<typename ...Args> void Log::v(Args&&... args) const { LOGV_IMPL("%s", Logger(std::forward<Args>(args)...).get().c_str()); }
template<typename ...Args> void Log::d(Args&&... args) const { LOGD_IMPL("%s", Logger(std::forward<Args>(args)...).get().c_str()); }
template<typename ...Args> void Log::i(Args&&... args) const { LOGI_IMPL("%s", Logger(std::forward<Args>(args)...).get().c_str()); }
template<typename ...Args> void Log::w(Args&&... args) const { LOGW_IMPL("%s", Logger(std::forward<Args>(args)...).get().c_str()); }
template<typename ...Args> void Log::e(Args&&... args) const { LOGE_IMPL("%s", Logger(std::forward<Args>(args)...).get().c_str()); }

}

#ifdef VCCC_LOG_DEBUG
#define LOGV(...) vc::Log::getInstance().v(__VA_ARGS__)
#define LOGD(...) vc::Log::getInstance().d(__VA_ARGS__)
#define LOGI(...) vc::Log::getInstance().i(__VA_ARGS__)
#define LOGW(...) vc::Log::getInstance().w(__VA_ARGS__)
#define LOGE(...) vc::Log::getInstance().e(__VA_ARGS__)
#define LOGF(...) LOGF_IMPL(__VA_ARGS__)
#define LOGS(...) LOGS_IMPL(__VA_ARGS__)
#else
#define LOGV(...) LOGV_IMPL(__VA_ARGS__)
  #define LOGD(...) LOGD_IMPL(__VA_ARGS__)
  #define LOGI(...) LOGI_IMPL(__VA_ARGS__)
  #define LOGW(...) LOGW_IMPL(__VA_ARGS__)
  #define LOGE(...) LOGE_IMPL(__VA_ARGS__)
  #define LOGF(...) LOGF_IMPL(__VA_ARGS__)
  #define LOGS(...) LOGS_IMPL(__VA_ARGS__)
#endif

#endif //VCCC_LOG_LOG_HPP
