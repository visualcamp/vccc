//
// Created by YongGyu Lee on 2020/12/02.
//

#ifndef VCPP_UTILITY_TIME_HPP
#define VCPP_UTILITY_TIME_HPP

#include <chrono>

namespace vc{

template<typename T>
auto getCurrentTime() {
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  return std::chrono::duration_cast<T>(duration).count();
}

decltype(auto) getCurrentMilliseconds() {
  return getCurrentTime<std::chrono::milliseconds>();
}

decltype(auto) getCurrentMicroseconds() {
  return getCurrentTime<std::chrono::microseconds>();
}

decltype(auto) getCurrentNanoseconds() {
  return getCurrentTime<std::chrono::nanoseconds>();
}

}

#endif //VCPP_UTILITY_TIME_HPP
