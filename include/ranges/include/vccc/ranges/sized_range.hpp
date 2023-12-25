//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_SIZED_RANGE_HPP_
#define VCCC_RANGES_SIZED_RANGE_HPP_

#include <type_traits>

#include "vccc/ranges/range.hpp"
#include "vccc/ranges/size.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename T, bool = range<T>::value, typename = void>
struct sized_range_impl : std::false_type {};

template<typename T>
struct sized_range_impl<
        T, true,
        void_t<decltype( ranges::size( std::declval<std::add_lvalue_reference_t<std::remove_reference_t<T>>>() ) )>>
    : std::true_type {};

} // namespace detail

/// @addtogroup ranges
/// @{

/**
@brief specifies that a range knows its size in constant time

The `%sized_range` concept specifies the requirements of a `ranges::range` type that knows its size in constant time with the `ranges::size` function.

 */

template<typename T>
struct sized_range : detail::sized_range_impl<T> {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_SIZED_RANGE_HPP_
