//
// Created by YongGyu Lee on 3/20/24.
//

#ifndef VCCC_MEMORY_DESTROY_AT_HPP_
#define VCCC_MEMORY_DESTROY_AT_HPP_

#include <type_traits>

#include "vccc/__core/constexpr.hpp"
#include "vccc/__memory/addressof.hpp"

namespace vccc {

template <typename T, std::enable_if_t<!std::is_array<T>::value, int> = 0>
VCCC_CONSTEXPR_AFTER_CXX20 void destroy_at(T* p) {
  p->~T();
}

template <typename T, std::enable_if_t<std::is_array<T>::value, int> = 0>
VCCC_CONSTEXPR_AFTER_CXX20 void destroy_at(T* p) {
  auto first = std::begin(*p);
  auto last = std::end(*p);

  for (; first != last; ++first) {
    vccc::destroy_at(vccc::addressof(*first));
  }
}

} // namespace vccc

#endif // VCCC_MEMORY_DESTROY_AT_HPP_
