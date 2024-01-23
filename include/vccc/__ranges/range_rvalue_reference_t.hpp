//
// Created by yonggyulee on 2024/01/22.
//

#ifndef VCCC_RANGES_RANGE_RVALUE_REFERENCE_T_HPP
#define VCCC_RANGES_RANGE_RVALUE_REFERENCE_T_HPP

#include "vccc/__iterator/iter_rvalue_reference_t.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/range.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename R, bool = range<R>::value /* false */>
struct range_rvalue_reference_impl {};

template<typename R>
struct range_rvalue_reference_impl<R, true> {
 using type = iter_rvalue_reference_t<iterator_t<R>>;
};

} // namespace detail

/// @addtogroup ranges
/// @{

template<typename R>
struct range_rvalue_reference : detail::range_rvalue_reference_impl<R> {};

/**
\brief Used to obtain the rvalue reference type of the iterator type of range type `R`.

@sa [std::ranges::range_rvalue_reference_t](https://en.cppreference.com/w/cpp/ranges/iterator_t)
 */
template<typename R>
using range_rvalue_reference_t = typename range_rvalue_reference<R>::type;

/// @}

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_RANGE_RVALUE_REFERENCE_T_HPP
