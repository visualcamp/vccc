# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_LOG_LOG_HPP
# define VCCC_LOG_LOG_HPP
#
# include "vccc/log/detail/platform_log.hpp"
# include "vccc/log/logger.hpp"

//! @addtogroup log
//! @{

namespace vccc {



/**
@brief logs the given message

v for LOGV, d for LOGD, i for LOGI, w for LOGW, e for LOGE

supports both printf-like format and ostringstream& operator <<
@code
    Log.d("Hello, world")                   // Hello, world
    Log.d("First:", 3, "Second:", 3.14)     // First: 3 Second: 3.14
    Log.d("Hello, %s", "world")             // Hello, world
    Log.d("First: %d Second: %f", 3, 3.14)  // First: 3 Second: 3.14
@endcode

Mixing both won't work
@code
    Log.d("First: %d", 3, "Second:", 3.14)  // First: 3 (wrong output; works exactly the same as printf)
    Log.d("First:", 3, "Second: %f", 3.14)  // First: 3 Second: %f 3.14 (wrong output; works exactly the same as std::cout)
@endcode

printf-like format only works when
     1. first argument is (const) char* type
     2. following arguments are scalar (pointer, integer, floating-points, enum)
     and of course the output will be wrong if the wrong format is provided

@code
    Log.d("Hello, %s", std::string("world"))    // Hello, %s world
    Log.d(std::string("Hello, %s"), "world")    // Hello, %s world
    Log.d("Int: %d", 3.14)                      // Int: -492425104
@endcode

if ostringstream& operator << is overloaded for user-defined types, it can be printed
@code
    Log.d("string:", std::string("wow!"))   // string: wow!
    Log.d("Point:", cv::Point2i(3,4))       // Point: [3, 4]
@endcode

if using << format, blank will be inserted between all elements
@code
    Log.d("ABCDEF")         // ABCDEF
    Log.d("ABC", "DEF")     // ABC DEF
    Log.d("AB", "CD", 12)   // AB CD 12
@endcode

* std::pair, std::tuple, std::integer_sequence, containers(non default-printable) are overloaded, and can be printed.
 */

class Log_{
 public:
  Log_() = default;

  template<typename ...Args> void Log_::v(Args&&... args) const { LOGV_IMPL("%s", Logger(std::forward<Args>(args)...).get().c_str()); }
  template<typename ...Args> void Log_::d(Args&&... args) const { LOGD_IMPL("%s", Logger(std::forward<Args>(args)...).get().c_str()); }
  template<typename ...Args> void Log_::i(Args&&... args) const { LOGI_IMPL("%s", Logger(std::forward<Args>(args)...).get().c_str()); }
  template<typename ...Args> void Log_::w(Args&&... args) const { LOGW_IMPL("%s", Logger(std::forward<Args>(args)...).get().c_str()); }
  template<typename ...Args> void Log_::e(Args&&... args) const { LOGE_IMPL("%s", Logger(std::forward<Args>(args)...).get().c_str()); }
};

static Log_ Log;

}

/** @brief macro wrappers for security */

# ifdef VCCC_LOG_DEBUG
  # define LOGV(...) ::vccc::Log.v(__VA_ARGS__)
  # define LOGD(...) ::vccc::Log.d(__VA_ARGS__)
  # define LOGI(...) ::vccc::Log.i(__VA_ARGS__)
  # define LOGW(...) ::vccc::Log.w(__VA_ARGS__)
  # define LOGE(...) ::vccc::Log.e(__VA_ARGS__)
  # define LOGF(...) LOGF_IMPL(__VA_ARGS__)
  # define LOGS(...) LOGS_IMPL(__VA_ARGS__)
# else
  # define LOGV(...) LOGV_IMPL(__VA_ARGS__)
  # define LOGD(...) LOGD_IMPL(__VA_ARGS__)
  # define LOGI(...) LOGI_IMPL(__VA_ARGS__)
  # define LOGW(...) LOGW_IMPL(__VA_ARGS__)
  # define LOGE(...) LOGE_IMPL(__VA_ARGS__)
  # define LOGF(...) LOGF_IMPL(__VA_ARGS__)
  # define LOGS(...) LOGS_IMPL(__VA_ARGS__)
# endif

//! @} log

# endif //VCCC_LOG_LOG_HPP
