//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_END_HPP_
#define VCCC_RANGES_END_HPP_

#include <cstddef>
#include <type_traits>

#include "vccc/iterator/input_or_output_iterator.hpp"
#include "vccc/iterator/sentinel_for.hpp"
#include "vccc/type_traits/detail/return_category.hpp"
#include "vccc/ranges/borrowed_range.hpp"
#include "vccc/ranges/decay_copy.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/type_traits/disjunction.hpp"
#include "vccc/type_traits/is_bounded_array.hpp"
#include "vccc/type_traits/is_complete.hpp"
#include "vccc/type_traits/remove_cvref.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

using vccc::detail::return_category;

template<typename T, bool = is_bounded_array<remove_cvref_t<T>>::value>
struct end_array_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct end_array_check<T, true> : std::true_type {
  using category = return_category<1, decltype(std::declval<T>() + std::extent<std::remove_reference_t<T>>::value)>;
};

template<typename T, typename = void>
struct end_member_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct end_member_check<T, void_t<decltype(vccc_decay_copy(std::declval<T>().end()))>>
    : concepts::sentinel_for<decltype(vccc_decay_copy(std::declval<T>().end())), ranges::iterator_t<T>> {
  using category = return_category<2, decltype(vccc_decay_copy(std::declval<T>().end()))>;
};

template<typename T, typename = void>
struct end_global_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct end_global_check<T, void_t<decltype(vccc_decay_copy(end(std::declval<T>())))>>
    : concepts::sentinel_for<decltype(vccc_decay_copy(end(std::declval<T>()))), ranges::iterator_t<T>> {
  using category = return_category<3, decltype(vccc_decay_copy(end(std::declval<T>())))>;
};

template<typename T>
struct end_category_impl
    : std::conditional_t<
        end_array_check<T>::value, typename end_array_check<T>::category,
      std::conditional_t<
        end_member_check<T>::value, typename end_member_check<T>::category,
      std::conditional_t<
        end_global_check<T>::value, typename end_global_check<T>::category,
        return_category<0>
      >>> {};


template<typename T>
struct end_category
    : std::conditional_t<
        disjunction<
          std::is_lvalue_reference<T>,
          enable_borrowed_range<std::remove_cv_t<T>>
        >::value,
        end_category_impl<T>,
        return_category<0>
      > {};

template<typename T, typename R>
constexpr R end(T&& t, return_category<1, R>) {
  static_assert(is_complete<std::remove_all_extents_t<std::remove_reference_t<T>>>::value, "Array element must be complete type");
  return t + std::extent<T>::value;
}

template<typename T, typename R>
constexpr R end(T&& t, return_category<2, R>) {
  return vccc_decay_copy(std::forward<T>(t).end());
}

template<typename T, typename R>
constexpr R end(T&& t, return_category<3, R>) {
  return vccc_decay_copy(end(std::forward<T>(t)));
}

} // namespace detail

template<typename T>
constexpr typename detail::end_category<T>::return_type
end(T&& t) {
  return detail::end(std::forward<T>(t), detail::end_category<T>{});
}


} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_END_HPP_