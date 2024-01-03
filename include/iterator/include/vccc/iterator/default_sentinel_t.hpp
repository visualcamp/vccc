//
// Created by cosge on 2024-01-03.
//

#ifndef VCCC_ITERATOR_DEFAULT_SENTINEL_T_HPP
#define VCCC_ITERATOR_DEFAULT_SENTINEL_T_HPP

#include <type_traits>

#include "vccc/core/inline_or_static.hpp"

namespace vccc {

/// @addtogroup iterator
/// @{

/**
@breif default sentinel for use with iterators that know the bound of their range

`%default_sentinel_t` is an empty class type used to denote the end of a range. It can be used together with iterator
types that know the bound of their range (e.g., `vccc::counted_iterator`).
 */
struct default_sentinel_t {};

VCCC_INLINE_OR_STATIC constexpr default_sentinel_t default_sentinel{};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_DEFAULT_SENTINEL_T_HPP
