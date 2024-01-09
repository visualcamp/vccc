//
// Created by yonggyulee on 2023/12/30.
//

#ifndef VCCC_RANGES_CONTIGUOUS_RANGE_HPP
#define VCCC_RANGES_CONTIGUOUS_RANGE_HPP

#include <type_traits>

#include "vccc/__concepts/same_as.hpp"
#include "vccc/__iterator/contiguous_iterator.hpp"
#include "vccc/__ranges/data.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/random_access_range.hpp"
#include "vccc/__ranges/range_reference_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"
#include "vccc/__type_traits/is_invocable.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<
    typename T,
    bool =
        conjunction<
            contiguous_iterator< ranges::iterator_t<T> >,
            is_invocable<decltype(ranges::data), T&>
        >::value /* true */
>
struct contiguous_range_impl_2
    : same_as<
          decltype( ranges::data(std::declval<T&>()) ),
          std::add_pointer_t<ranges::range_reference_t<T>>
      > {};
template<typename T>
struct contiguous_range_impl_2<T, false> : std::false_type {};

template<
    typename T,
    bool =
        conjunction<
            random_access_range<T>,
            has_typename_type< ranges::iterator<T> >,
            has_typename_type< ranges::range_reference<T> >
        >::value /* true */
>
struct contiguous_range_impl_1 : contiguous_range_impl_2<T> {};
template<typename T>
struct contiguous_range_impl_1<T, false> : std::false_type {};

} // namespace ranges

/// @addtogroup ranges
/// @{

/**
\brief specifies a range whose iterator type satisfies `contiguous_iterator`

The `%contiguous_range` concept is a refinement of \ref ranges::range "range" for which `ranges::begin` returns a model
of `contiguous_iterator` and the customization point `ranges::data` is usable.

@sa [std::ranges::contiguous_range](https://en.cppreference.com/w/cpp/ranges/contiguous_range)
@sa `ranges::sized_range`
@sa `ranges::random_access_range`
 */
template<typename T>
struct contiguous_range : detail::contiguous_range_impl_1<T> {};

/// @}

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_CONTIGUOUS_RANGE_HPP
