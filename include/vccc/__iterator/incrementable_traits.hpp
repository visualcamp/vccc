//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_ITERATOR_INCREMENTABLE_TRAITS_HPP_
#define VCCC_ITERATOR_INCREMENTABLE_TRAITS_HPP_

#include <cstddef>
#include <iterator>
#include <type_traits>

#include "vccc/__concepts/subtractable.hpp"
#include "vccc/__type_traits/empty.hpp"
#include "vccc/__type_traits/has_typename_difference_type.hpp"

namespace vccc {
namespace detail {

template<typename T, bool = subtractable<T>::value>
struct substract_check : std::false_type {};

template<typename T>
struct substract_check<T, true> : std::is_integral<decltype(std::declval<const T&>() - std::declval<const T&>())> {};


template<typename T>
struct incrementable_traits_subtractable {
  using difference_type = std::make_signed_t<decltype(std::declval<T>() - std::declval<T>())>;
};

template<typename T>
struct incrementable_traits_no_difference_type
    : std::conditional_t<
        substract_check<T>::value,
        incrementable_traits_subtractable<T>,
        empty
      > {};

template<typename T>
struct incrementable_traits_yes_difference_type {
  using difference_type = typename T::difference_type;
};

template<typename T>
struct incrementable_traits_impl
    : std::conditional_t<
        has_typename_difference_type<T>::value,
        incrementable_traits_yes_difference_type<T>,
        incrementable_traits_no_difference_type<T>
      > {};

template<typename T>
struct incrementable_traits_object_pointer {
  using difference_type = std::ptrdiff_t;
};

template<typename T>
struct incrementable_traits_impl<T*>
    : std::conditional_t<
        std::is_object<T>::value,
        incrementable_traits_object_pointer<T>,
        empty
      > {};

#if __cplusplus < 202002L
template<typename T>
struct incrementable_traits_impl<std::back_insert_iterator<T>> {
  using difference_type = std::ptrdiff_t;
};

template<typename T>
struct incrementable_traits_impl<std::front_insert_iterator<T>> {
  using difference_type = std::ptrdiff_t;
};

template<typename T>
struct incrementable_traits_impl<std::insert_iterator<T>> {
  using difference_type = std::ptrdiff_t;
};

template<typename T>
struct incrementable_traits_impl<std::ostream_iterator<T>> {
  using difference_type = std::ptrdiff_t;
};

template<typename T>
struct incrementable_traits_impl<std::ostreambuf_iterator<T>> {
  using difference_type = std::ptrdiff_t;
};
#endif

} // namespace detail


/// @addtogroup iterator
/// @{

/**
 * \brief computes the difference type of a weakly_incrementable type
 *
 * Computes the associated difference type of the type `I`, if any.
 * Users may specialize `incrementable_traits` for a program-defined type. <br/>
 *
 * -# Primary template is an empty struct. <br/>
 * -# Specialization for pointers. Provides a member type `difference_type` equal to `std::ptrdiff_t`. <br/>
 * -# Specialization for const-qualified types. <br/>
 * -# Specialization for types that define a public and accessible member type `difference_type`.
 * Provides a member type `difference_type` equal to `T::difference_type`. <br/>
 * -# Specialization for types that do not define a public and accessible member type `difference_type`
 * but do support subtraction. Provides a member type `difference_type` equal to
 * `std::make_signed_t<decltype(std::declval<T>() - std::declval<T>())>`. <br/>
 *
 * See [std::incrementable_traits](https://en.cppreference.com/w/cpp/iterator/incrementable_traits) for more details
 * \tparam I
 */

template<typename I>
struct incrementable_traits;

/// @}

template<typename I>
struct incrementable_traits : detail::incrementable_traits_impl<I> {};

template<typename T>
struct incrementable_traits<const T> : incrementable_traits<T> {};

} // namespace vccc

#endif // VCCC_ITERATOR_INCREMENTABLE_TRAITS_HPP_
