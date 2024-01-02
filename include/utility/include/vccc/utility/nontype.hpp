//
// Created by yonggyulee on 2024/01/02.
//

#ifndef VCCC_UTILITY_NONTYPE_HPP
#define VCCC_UTILITY_NONTYPE_HPP

#include <type_traits>

#include "vccc/core/inline_or_static.hpp"

namespace vccc {

/// @addtogroup utility
/// @{


/**
@brief value construction tag

@sa [std::nontype](https://en.cppreference.com/w/cpp/utility/nontype)
 */

template<typename V>
struct nontype_t {
  constexpr explicit nontype_t() = default;
};

template<typename T>
VCCC_INLINE_OR_STATIC constexpr nontype_t<T> nontype{};

/// @}

} // namespace vccc

#endif // VCCC_UTILITY_NONTYPE_HPP
