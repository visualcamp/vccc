//
// Created by yonggyulee on 1/25/24.
//

#ifndef VCCC_CONCEPTS_VALUE_SWAPPABLE_HPP
#define VCCC_CONCEPTS_VALUE_SWAPPABLE_HPP

#include <type_traits>

#include "vccc/__concepts/dereferenceable.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_swappable.hpp"

namespace vccc {
namespace detail {

template<typename I1, typename I2, bool = conjunction<dereferenceable<I1>, dereferenceable<I2>>::value>
struct ValueSwappableImpl : std::false_type {};
template<typename I1, typename I2>
struct ValueSwappableImpl<I1, I2, true>
    : is_swappable<decltype(*std::declval<I1&>(), *std::declval<I2&>())> {};

} // namespace detail

/// @addtogroup concepts
/// @{

template<typename I1, typename I2>
struct ValueSwappable : detail::ValueSwappableImpl<I1, I2> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_VALUE_SWAPPABLE_HPP
