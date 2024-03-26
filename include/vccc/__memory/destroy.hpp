//
// Created by YongGyu Lee on 3/20/24.
//

#ifndef VCCC_MEMORY_DESTROY_HPP_
#define VCCC_MEMORY_DESTROY_HPP_

#include "vccc/__core/constexpr.hpp"
#include "vccc/__memory/addressof.hpp"
#include "vccc/__memory/destroy_at.hpp"

namespace vccc {

template<typename ForwardIt>
VCCC_CONSTEXPR_AFTER_CXX20 void destroy(ForwardIt first, ForwardIt last) {
  for (; first != last; ++first) {
    vccc::destroy_at(vccc::addressof(*first));
  }
}


} // namespace vccc

#endif // VCCC_MEMORY_DESTROY_HPP_
