//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_ITERATOR_INDIRECTLY_READABLE_TRAITS_HPP_
#define VCCC_ITERATOR_INDIRECTLY_READABLE_TRAITS_HPP_

#include <type_traits>

#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/empty.hpp"
#include "vccc/__type_traits/has_typename_element_type.hpp"
#include "vccc/__type_traits/has_typename_value_type.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace detail {

template<typename T>
struct value_typer {
  using value_type = T;
};

template<
    typename T,
    bool = has_typename_value_type<T>::value,
    bool = has_typename_element_type<T>::value
>
struct indirectly_readable_traits_impl {};

template<typename T>
struct indirectly_readable_traits_impl<T, true, false>
    : std::conditional_t<
        std::is_object<typename T::value_type>::value,
        value_typer<std::remove_cv_t<typename T::value_type>>,
        empty
      > {};

template<typename T>
struct indirectly_readable_traits_impl<T, false, true>
    : std::conditional_t<
        std::is_object<typename T::element_type>::value,
        value_typer<std::remove_cv_t<typename T::element_type>>,
        empty
      > {};

template<typename T>
struct indirectly_readable_traits_impl<T, true, true>
    : std::conditional_t<
        conjunction<
          std::is_object<typename T::value_type>, std::is_object<typename T::element_type>,
          std::is_same<std::remove_cv_t<typename T::value_type>, std::remove_cv_t<typename T::element_type>>
        >::value,
        value_typer<std::remove_cv_t<typename T::value_type>>,
        empty
      > {};

} // namespace detail

/// @addtogroup iterator
/// @{

template<typename I>
struct indirectly_readable_traits : detail::indirectly_readable_traits_impl<I> {};

/// @}

template<typename T>
struct indirectly_readable_traits<T*>
    : std::conditional_t<
        std::is_object<T>::value,
        detail::value_typer<remove_cvref_t<T>>,
        empty
      > {};

template<typename T>
struct indirectly_readable_traits<const T> : indirectly_readable_traits<T> {};

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECTLY_READABLE_TRAITS_HPP_
