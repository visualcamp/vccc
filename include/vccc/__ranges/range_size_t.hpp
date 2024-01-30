//
// Created by yonggyulee on 2024/01/06.
//

#ifndef VCCC_RANGES_RANGE_SIZE_T_HPP
#define VCCC_RANGES_RANGE_SIZE_T_HPP

#include <type_traits>

#include "vccc/__ranges/size.hpp"
#include "vccc/__ranges/sized_range.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename R, bool = sized_range<R>::value /* true */>
struct range_size_impl {
  using type = decltype(ranges::size(std::declval<R&>()));
};

template<typename R>
struct range_size_impl<R, false> {};

} // namespace detail

template<typename R>
struct range_size : detail::range_size_impl<R> {};

/// @addtogroup ranges
/// @{

/**
\brief Used to obtain the size type of the \ref ranges::sized_range "sized_range" type R.

@sa [std::ranges::range_size_t](https://en.cppreference.com/w/cpp/ranges/iterator_t)
 */
template<typename R>
using range_size_t = typename range_size<R>::type;

/// @}

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_RANGE_SIZE_T_HPP
