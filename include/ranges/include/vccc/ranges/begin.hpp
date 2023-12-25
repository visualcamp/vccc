//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_BEGIN_HPP_
#define VCCC_RANGES_BEGIN_HPP_

#include <cstddef>
#include <type_traits>

#include "vccc/iterator/input_or_output_iterator.hpp"
#include "vccc/type_traits/detail/return_category.hpp"
#include "vccc/ranges/borrowed_range.hpp"
#include "vccc/ranges/decay_copy.hpp"
#include "vccc/type_traits/disjunction.hpp"
#include "vccc/type_traits/is_complete.hpp"
#include "vccc/type_traits/remove_cvref.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

using vccc::detail::return_category;

template<typename T, bool = std::is_array<remove_cvref_t<T>>::value>
struct begin_array_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct begin_array_check<T, true> : std::true_type {
  using category = return_category<1, decltype(std::declval<T>() + 0)>;
};

template<typename T, typename = void>
struct begin_member_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct begin_member_check<T, void_t<decltype(vccc_decay_copy(std::declval<T>().begin()))>>
    : concepts::input_or_output_iterator<decltype(vccc_decay_copy(std::declval<T>().begin()))> {
  using category = return_category<2, decltype(vccc_decay_copy(std::declval<T>().begin()))>;
};

template<typename T, typename = void>
struct begin_global_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct begin_global_check<T, void_t<decltype(vccc_decay_copy(begin(std::declval<T>())))>>
    : concepts::input_or_output_iterator<decltype(vccc_decay_copy(begin(std::declval<T>())))> {
  using category = return_category<3, decltype(vccc_decay_copy(begin(std::declval<T>())))>;
};

template<typename T>
struct begin_category_impl
    : std::conditional_t<
        begin_array_check<T>::value, typename begin_array_check<T>::category,
      std::conditional_t<
        begin_member_check<T>::value, typename begin_member_check<T>::category,
      std::conditional_t<
        begin_global_check<T>::value, typename begin_global_check<T>::category,
        return_category<0>
      >>> {};


template<typename T>
struct begin_category
    : std::conditional_t<
        disjunction<
          std::is_lvalue_reference<T>,
          enable_borrowed_range<std::remove_cv_t<T>>
        >::value,
        begin_category_impl<T>,
        return_category<0>
      > {};

template<typename T, typename R>
constexpr R begin(T&& t, return_category<1, R>) {
  static_assert(is_complete<std::remove_all_extents_t<std::remove_reference_t<T>>>::value, "Array element must be complete type");
  return t + 0;
}

template<typename T, typename R>
constexpr R begin(T&& t, return_category<2, R>) {
  return vccc_decay_copy(std::forward<T>(t).begin());
}

template<typename T, typename R>
constexpr R begin(T&& t, return_category<3, R>) {
  return vccc_decay_copy(begin(std::forward<T>(t)));
}

} // namespace detail

template<typename T>
constexpr typename detail::begin_category<T>::return_type
begin(T&& t) {
  return detail::begin(std::forward<T>(t), detail::begin_category<T>{});
}


} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_BEGIN_HPP_
