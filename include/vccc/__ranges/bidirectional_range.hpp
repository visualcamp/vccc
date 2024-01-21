//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_BIDIRECTIONAL_RANGE_HPP_
#define VCCC_RANGES_BIDIRECTIONAL_RANGE_HPP_

#include <type_traits>

#include "vccc/__iterator/bidirectional_iterator.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<
    typename T,
    bool =
        conjunction<
          ranges::forward_range<T>,
          has_typename_type<ranges::iterator<T>>
        >::value
>
struct bidirectional_range_impl : std::false_type {};

template<typename T>
struct bidirectional_range_impl<T, true> : bidirectional_iterator<ranges::iterator_t<T>> {};

} // namespace ranges

/// @addtogroup ranges
/// @{

/**
\brief specifies a range whose iterator type satisfies `bidirectional_iterator`

The `%bidirectional_range` concept is a refinement of range for which `ranges::begin` returns a model of
`bidirectional_iterator`.

@sa [std::ranges::bidirectional_range](https://en.cppreference.com/w/cpp/ranges/bidirectional_range)
@sa `ranges::forward_range`
@sa `ranges::random_access_range`
 */
template<typename T>
struct bidirectional_range : detail::bidirectional_range_impl<T> {};


/// @}

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_BIDIRECTIONAL_RANGE_HPP_
