//
// Created by YongGyu Lee on 2020/12/07.
//

#ifndef VCCC_UTILITY_BIND_AT_HPP
#define VCCC_UTILITY_BIND_AT_HPP

#include "vccc/utility/at.hpp"

namespace vc{
/**
 *  call at<...>(t) to args...
 */

namespace detail{

template<typename ...Ts>
class bind_obj {
 public:
  template<typename ...Args>
  constexpr bind_obj(Args&&... args) : tup(std::forward<Args>(args)...) {}

  template<typename T>
  constexpr bind_obj& operator = (T&& type) {
    bind_impl(std::forward<T>(type), std::index_sequence_for<Ts...>{});
    return *this;
  }

 private:
  template<typename T, std::size_t ...I>
  constexpr void bind_impl(T&& type, std::index_sequence<I...>) {
    [[maybe_unused]] volatile int dummy[sizeof...(I)] = {
        (at<I>(tup) = at<I>(type), 0)...
    };
  }


  std::tuple<Ts...> tup;
};

}

template<typename ...Args>
constexpr detail::bind_obj<Args...> bind_at(Args&&... args) {
  return detail::bind_obj<Args...>(std::forward<Args>(args)...);
}

}

#endif //VCCC_UTILITY_BIND_AT_HPP
