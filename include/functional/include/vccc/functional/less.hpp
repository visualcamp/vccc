//
// Created by YongGyu Lee on 2024/01/03.
//

#ifndef VCCC_FUNCTIONAL_LESS_HPP
#define VCCC_FUNCTIONAL_LESS_HPP

#include <type_traits>
#include <utility>

#include "vccc/concepts/totally_ordered.hpp"
#include "vccc/utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup functional
/// @{

struct less {
  using is_transparent = std::true_type;

  template<typename T, typename U, std::enable_if_t<totally_ordered_with<T, U>::value, int> = 0>
  constexpr bool operator()(T&& t, U&& u) const
      noexcept(noexcept(bool(std::forward<T>(t) < std::forward<U>(u))))
  {
    using namespace vccc::rel_ops;
    return std::forward<T>(t) < std::forward<U>(u);
  }
};


/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_FUNCTIONAL_LESS_HPP
