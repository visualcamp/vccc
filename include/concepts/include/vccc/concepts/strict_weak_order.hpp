//
// Created by yonggyulee on 2023/12/27.
//

#ifndef VCCC_CONCEPTS_STRICT_WEAK_ORDER_HPP
#define VCCC_CONCEPTS_STRICT_WEAK_ORDER_HPP

#include "vccc/concepts/relation.hpp"

namespace vccc {
/// @addtogroup concepts
/// @{

/**
@brief specifies that a relation imposes a strict weak ordering

The concept `%strict_weak_order<R, T, U>` specifies that the `relation` `R` imposes a strict weak ordering on its arguments.

@sa [std::strict_weak_order](https://en.cppreference.com/w/cpp/concepts/strict_weak_order)
 */
template<typename R, typename T, typename U>
struct strict_weak_order : relation<R, T, U> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_STRICT_WEAK_ORDER_HPP
