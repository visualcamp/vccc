# /*
#  * Created by YongGyu Lee on 2021/03/17.
#  */
#
# ifndef VCCC_LOG_STRINGFY_CHRONO_HPP_
# define VCCC_LOG_STRINGFY_CHRONO_HPP_
#
# include "vccc/log/stringfy/forward_declare.h"
# include <chrono>
# include <sstream>
# include <iomanip>
# include <mutex>
# include <cmath>

namespace vccc {



template<typename Rep, typename Period>
std::string
stringfy(const std::chrono::duration<Rep, Period>& duration) {
  std::stringstream ss;

  if(duration < std::chrono::seconds(1).count()) {
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    ss << std::setprecision(5);

    if(ns < std::chrono::nanoseconds (1'000))
      ss << static_cast<long double>(ns.count()) << "ns";
    else if(ns < std::chrono::milliseconds (1'000))
      ss << static_cast<long double>(ns.count()) / 1'000 << "ms";
    else

  }
  else {

  }

  return ss.str();
}

template<typename Clock, typename Duration>
std::string
stringfy(const std::chrono::time_point<Clock, Duration>& time_point) {
  using system_clock = std::chrono::system_clock;

  static const auto system_clock_begin = system_clock::now();
  static const auto custom_clock_begin = Clock::now();
  static const auto gap =
      system_clock_begin.time_since_epoch() - custom_clock_begin.time_since_epoch();

  auto d = std::chrono::duration_cast<std::chrono::system_clock::duration>(time_point.time_since_epoch() + gap);
  return stringfy(std::chrono::time_point<std::chrono::system_clock>() + d);
}

template<typename Duration>
std::string stringfy(const std::chrono::time_point<std::chrono::system_clock, Duration>& time_point) {
  static std::mutex localtime_m;

  auto tt = std::chrono::system_clock::to_time_t(time_point);

  std::unique_lock<std::mutex> lck(localtime_m);
  const std::tm* tm_obj = std::localtime(&tt);

  if(tm_obj == nullptr) // failed parsing
    return std::to_string(tt);

  std::stringstream ss;
  ss << std::put_time(tm_obj, "%Y-%m-%d %H:%M:%S.");
  lck.unlock();

  // calculate milliseconds
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch())
      - std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch());
  ss << std::setfill('0') << std::setw(3) << ms.count();

  return ss.str();
}

std::string stringfy(const std::time_t* tt) {
  static std::mutex localtime_m;
  std::lock_guard<std::mutex> lck(localtime_m);

  const std::tm* tm_obj = std::localtime(tt);

  if(tm_obj == nullptr) // failed parsing
    return std::to_string(*tt);

  std::stringstream ss;
  ss << std::put_time(tm_obj, "%Y-%m-%d %H:%M:%S.");
  return ss.str();
}

} // namespace vccc

# endif //VCCC_LOG_STRINGFY_CHRONO_HPP_
