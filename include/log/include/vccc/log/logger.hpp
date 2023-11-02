# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_LOG_LOG_HPP
# define VCCC_LOG_LOG_HPP
#
# include "vccc/log/detail/log_impl.h"
# include "vccc/log/stream_wrapper.hpp"


namespace vccc {

//! @addtogroup log
//! @{

/**
@brief Ease use of vccc::StreamWrapper

d for LOGD, i for LOGI, w for LOGW, e for LOGE

supports ostringstream& operator <<
@code
    Log.d("Hello, world")                   // Hello, world
    Log.d("First: ", 3, " Second: ", 3.14)     // First: 3 Second: 3.14
@endcode

To use printf-like format, use vccc::Formatter

if ostringstream& operator << is overloaded for user-defined types, it can be printed
@code
    Log.d("string: ", std::string("wow!"))   // string: wow!
    Log.d("Point: ", cv::Point2i(3,4))       // Point: [3, 4]
@endcode

* std::pair, std::tuple, std::integer_sequence, containers, chrono types, aggregate(since C++17) types can be printed.

@code
    Log.d(std::vector<std::vector<int>>{{1, 2}, {3}});  // { { 1, 2 }, { 3 } }
    Log.d(std::make_tuple(1, "hello"));                 // { 1, hello }
    Log.d(std::chrono::system_clock::now());            // 2021-06-29 14:35:27.917

    // Since C++ 17
    struct foo {
      int x;
      std::string name;
    };
    Log.d(foo{100, "Tony"});    // { 100, Tony }
@endcode

 */
class Logger {
 public:
  using stream_type = StreamWrapper;
  using string_type = std::string;

  constexpr Logger() = default;

  /** @brief Log output as debug */
  template<typename ...Args> void d(const Args&... args) const { d_(to_string(args...)); }
  /** @brief Informational log */
  template<typename ...Args> void i(const Args&... args) const { i_(to_string(args...)); }
  /** @brief Warning log */
  template<typename ...Args> void w(const Args&... args) const { w_(to_string(args...)); }
  /** @brief Error log */
  template<typename ...Args> void e(const Args&... args) const { e_(to_string(args...)); }

  /** @brief Return logged value as std::string
   *
   * @tparam Args
   * @param args
   * @return Logged value as std::string
   */
  template<typename ...Args> string_type to_string(const Args&... args) const {
    stream_type stream;
    int dummy[] = { (stream << args, 0)... };
    return stream.stream().str();
  }

  /** @brief Equals to stream_type::global_separator
   *
   * @return stream_type::global_separator()
   */
  static string_type& global_separator() {
    return stream_type::global_separator();
  }

 private:
  template<typename ...Args> void d_(const std::string& str) const { LOGD_IMPL("%s", str.c_str()); }
  template<typename ...Args> void i_(const std::string& str) const { LOGI_IMPL("%s", str.c_str()); }
  template<typename ...Args> void w_(const std::string& str) const { LOGW_IMPL("%s", str.c_str()); }
  template<typename ...Args> void e_(const std::string& str) const { LOGE_IMPL("%s", str.c_str()); }
};

/**
@brief Global vccc::Logger instance for syntax sugar
 */
constexpr Logger Log;

/// @defgroup log_log_macro__macro__Logging_macros LOGD, LOGI, LOGW, LOGE
/// @{

# ifdef NDEBUG
# define LOGD(...)
#else
/**
 *
@brief Debug log wrapper for security. This won't print in release build.

Below example applies to LOGI, LOGW, LOGE
@code{.cpp}
    LOGD("string ", 100, " ", 3.14);        // string 100 3.14
    LOGD(std::vector<int>{1,2,3,4});        // { 1, 2, 3, 4 }
    LOGD(std::map<string, int>{{"one", 1}, {"two", 2}}); // { { one: 1 }, { two: 2 } }
@endcode
 */
# define LOGD(...) ::vccc::Log.d(__VA_ARGS__)
# endif

/** @brief Information log wrapper **/
# define LOGI(...) ::vccc::Log.i(__VA_ARGS__)
/** @brief Warning log wrapper **/
# define LOGW(...) ::vccc::Log.w(__VA_ARGS__)
/** @brief Error log wrapper **/
# define LOGE(...) ::vccc::Log.e(__VA_ARGS__)
/// @}

//! @} log

} // namespace log

# endif //VCCC_LOG_LOG_HPP
