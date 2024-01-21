//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_COMMON_RANGE_HPP_
#define VCCC_RANGES_COMMON_RANGE_HPP_

#include <type_traits>

#include "vccc/__concepts/same_as.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/sentinel_t.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<
    typename T,
    bool =
        conjunction<
          ranges::range<T>,
          has_typename_type<ranges::iterator<T>>,
          has_typename_type<ranges::sentinel<T>>
        >::value
>
struct common_range_impl : std::false_type {};

template<typename T>
struct common_range_impl<T, true> : same_as<ranges::iterator_t<T>, ranges::sentinel_t<T>> {};

} // namespace ranges

template<typename T>
struct common_range : detail::common_range_impl<T> {};

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_COMMON_RANGE_HPP_
