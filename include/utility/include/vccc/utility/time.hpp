# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_UTILITY_TIME_HPP
# define VCCC_UTILITY_TIME_HPP
#
# include <chrono>

namespace vccc{

//! @addtogroup utility_time
//! @{

template<typename T>
auto
getCurrentTime()
{
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  return std::chrono::duration_cast<T>(duration).count();
}

inline
decltype(auto)
getCurrentMilliseconds()
{
  return getCurrentTime<std::chrono::milliseconds>();
}

inline
decltype(auto)
getCurrentMicroseconds()
{
  return getCurrentTime<std::chrono::microseconds>();
}

inline
decltype(auto)
getCurrentNanoseconds()
{
  return getCurrentTime<std::chrono::nanoseconds>();
}

//! @} utility_time

}

# endif //VCCC_UTILITY_TIME_HPP
