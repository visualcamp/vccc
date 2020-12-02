//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCPP_LOG_LOGGER_HPP
#define VCPP_LOG_LOGGER_HPP

#include <sstream>
#include <regex>
#include <string>

#include "vcpp/type_traits.hpp"
#include "vcpp/log/detail/platform_log.hpp"
#include "vcpp/log/detail/custom_ostream.hpp"


namespace vc {

namespace traits{

template<typename ...>
struct are_types_c_printable : std::false_type {};

template<typename T>
struct are_types_c_printable<T> : std::false_type {};

template<typename T, typename ...Ts>
struct are_types_c_printable<T, Ts...>
    : std::integral_constant<bool, std::is_same<char *, std::decay_t<T>>::value &&
        vc::are_scalar_v<Ts...>> {};
}

namespace impl{

using c_printable = std::true_type;
using not_c_printable = std::false_type;

class LoggerImpl {
 public:
  template<typename ...Args>
  LoggerImpl(const Args&... args);

  inline std::string get() const {
    return out.str();
  }

 private:
  template<typename CharT>
  static bool isFormatted(const CharT& str);

  template<typename Arg, typename ...Args>
  void addImpl(c_printable, const Arg& arg, const Args&... args);

  template<typename Arg, typename ...Args>
  void addImpl(not_c_printable, const Arg& arg, const Args&... args);

  template<typename Arg>
  void add(const Arg& n);

  template<typename Arg, typename ...Args>
  void add(const Arg& arg, const Args&... args);

  template<typename ...Args>
  void addFormatted(const char* fmt, const Args&... val);

  std::vector<char> buffer;
  std::stringstream out;
//  static std::string separator;
};

template<typename ...Args>
LoggerImpl::LoggerImpl(const Args& ...args) {
  addImpl(traits::are_types_c_printable<Args...>(), args...);
}


// TODO: move fmt_reg to class scope
template<typename CharT> bool LoggerImpl::isFormatted(const CharT& str) {
  static const std::regex fmt_reg(R"(%(?:\d+\$)?[+-]?(?:[ 0]|'.{1})?-?\d*(?:\.\d+)?[bcdeEufFgGosxXp])");
  return std::regex_search(str, fmt_reg);
}


template<typename Arg, typename ...Args>
void LoggerImpl::addImpl(c_printable, const Arg& arg, const Args&... args){
  if (isFormatted(arg))
    addFormatted(arg, args...);
  else
    addImpl(not_c_printable(), arg, args...);
}

template<typename Arg, typename ...Args>
void LoggerImpl::addImpl(not_c_printable, const Arg& arg, const Args&... args){
  return add(arg, args...);
}

template<typename T> void LoggerImpl::add(const T& n) {
  out << n;
}

template<typename Arg, typename ...Args> void LoggerImpl::add(const Arg& arg, const Args&... args) {
  add(arg);
  add(' '); // default separator
  add(args...);
}

template<typename ...Args>
void LoggerImpl::addFormatted(const char* fmt, const Args&... val) {
  ssize_t size = snprintf(NULL, 0, fmt, val...);
  if(buffer.size() < size + 1)    buffer.resize(size + 1);
  else if (buffer.size() > 100)   buffer.resize(100); // prevents too frequent resizing
  snprintf(buffer.data(), size + 1, fmt, val...);
  out << buffer.data();
}

}


class Logger{
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

  static inline Logger& getInstance() {
    static Logger logger;
    return logger;
  }

  template<typename ...Args> void v(Args&&... args) const;
  template<typename ...Args> void d(Args&&... args) const;
  template<typename ...Args> void i(Args&&... args) const;
  template<typename ...Args> void w(Args&&... args) const;
  template<typename ...Args> void e(Args&&... args) const;

 private:
  Logger() = default;

};

//static Logger Log;


template<typename ...Args> void Logger::v(Args&&... args) const { LOGV_IMPL("%s", impl::LoggerImpl(std::forward<Args>(args)...).get().c_str()); }
template<typename ...Args> void Logger::d(Args&&... args) const { LOGD_IMPL("%s", impl::LoggerImpl(std::forward<Args>(args)...).get().c_str()); }
template<typename ...Args> void Logger::i(Args&&... args) const { LOGI_IMPL("%s", impl::LoggerImpl(std::forward<Args>(args)...).get().c_str()); }
template<typename ...Args> void Logger::w(Args&&... args) const { LOGW_IMPL("%s", impl::LoggerImpl(std::forward<Args>(args)...).get().c_str()); }
template<typename ...Args> void Logger::e(Args&&... args) const { LOGE_IMPL("%s", impl::LoggerImpl(std::forward<Args>(args)...).get().c_str()); }

}

#ifdef VCPP_LOG_DEBUG
  #define LOGV(...) vc::Logger::getInstance().v(__VA_ARGS__)
  #define LOGD(...) vc::Logger::getInstance().d(__VA_ARGS__)
  #define LOGI(...) vc::Logger::getInstance().i(__VA_ARGS__)
  #define LOGW(...) vc::Logger::getInstance().w(__VA_ARGS__)
  #define LOGE(...) vc::Logger::getInstance().e(__VA_ARGS__)
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

#endif //VCPP_LOG_LOGGER_HPP
