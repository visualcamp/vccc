//
// Created by yonggyulee on 2023/12/26.
//

#ifndef VCCC_RANGES_RANDOM_ACCESS_RANGE_HPP_
#define VCCC_RANGES_RANDOM_ACCESS_RANGE_HPP_

#include <type_traits>

#include "vccc/iterator/random_access_iterator.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/ranges/bidirectional_range.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/has_typename_type.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<
    typename T,
    bool =
        conjunction<
          ranges::bidirectional_range<T>,
          has_typename_type<ranges::iterator<T>>
        >::value
>
struct random_access_range_impl : std::false_type {};

template<typename T>
struct random_access_range_impl : random_access_iterator<ranges::iterator_t<T>> {};

} // namespace ranges

/// @addtogroup ranges
/// @{

/**
\brief specifies a range whose iterator type satisfies `random_access_iterator`

The `%random_access_range` concept is a refinement of \ref ranges::range "range" for which `ranges::begin` returns a model of `random_access_iterator`.

@sa [std::ranges::random_access_range](https://en.cppreference.com/w/cpp/ranges/random_access_range)
@sa `ranges::sized_range`
@sa `ranges::contiguous_range`
 */
template<typename T>
struct random_access_range : detail::random_access_range_impl<T> {};


/// @}

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_RANDOM_ACCESS_RANGE_HPP_
