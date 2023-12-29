//
// Created by yonggyulee on 2023/12/30.
//

#ifndef VCCC_RANGES_ENABLE_VIEW_HPP
#define VCCC_RANGES_ENABLE_VIEW_HPP

#include <type_traits>

#include "vccc/concepts/derived_from.hpp"
#include "vccc/ranges/view_base.hpp"
#include "vccc/ranges/view_interface.hpp"
#include "vccc/type_traits/disjunction.hpp"
#include "vccc/type_traits/is_invocable.hpp"
#include "vccc/type_traits/is_referenceable.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct is_derived_from_view_interface_tester {
  template<typename T, typename U>
  auto operator()(const T&, const view_interface<U>&) -> std::is_same<T, view_interface<U>>;
};

template<typename T, bool = is_referencable<T>::value /* true */>
struct is_derived_from_view_interface
    : is_invocable_r<
          std::false_type, is_derived_from_view_interface_tester, T&, T&>{};

template<typename T>
struct is_derived_from_view_interface<T, false> : std::false_type {};

template<typename T>
struct enable_view_impl
    : disjunction<
          derived_from<T, view_base>,
          is_derived_from_view_interface<T>
      > {};

} // namespace detail

/// @addtogroup ranges
/// @{


/**
@brief The `%enable_view` variable template is used to indicate whether a \ref ranges::range "range" is a view.

<H1>Notes</H1>
`%vccc::ranges::enable_view` is implemented as `struct`(`std::ranges::enable_view` is `inline constexpr bool`).
Specializations must be inherited from `std::integral_constant<bool, v>`.

@sa [std::ranges::enable_view](https://en.cppreference.com/w/cpp/ranges/view)
@sa view_base
@sa view_interface
 */
template<typename T>
struct enable_view : detail::enable_view_impl<T> {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_ENABLE_VIEW_HPP
