//
// Created by YongGyu Lee on 3/27/24.
//

#ifndef VCCC_RANGES_OUTPUT_RANGE_HPP_
#define VCCC_RANGES_OUTPUT_RANGE_HPP_

#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__iterator/output_iterator.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename R, typename T, bool = range<R>::value /* true */>
struct output_range_impl : std::false_type {};

template<typename R, typename T>
struct output_range_impl<R, T, true> : output_iterator<iterator_t<R>, T> {};

} // namespace detail

/// @addtogroup ranges
/// @{

template<typename R, typename T>
struct output_range : detail::output_range_impl<R, T> {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_OUTPUT_RANGE_HPP_
