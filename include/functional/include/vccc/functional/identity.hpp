//
// Created by YongGyu Lee on 12/28/23.
//

#ifndef VCCC_FUNCTIONAL_IDENTITY_HPP
#define VCCC_FUNCTIONAL_IDENTITY_HPP

#include <utility>

namespace vccc {

/// @addtogroup functional
/// @{


/**
@brief 	function object that returns its argument unchanged

`%vccc::identity` is a function object type whose `operator()` returns its argument unchanged.

@sa[std::identity](https://en.cppreference.com/w/cpp/utility/functional/identity)
@sa type_traits_type_identity__class__Miscellaneous_transformations
 */
struct identity {
  template<typename T>
  constexpr T&& operator()(T&& t) const noexcept {
    return std::forward<T>(t);
  }
};

/// @}

} // namespace vccc

#endif // VCCC_FUNCTIONAL_IDENTITY_HPP
