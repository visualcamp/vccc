//
// Created by yonggyulee on 2023/12/31.
//

#ifndef VCCC_ITERATOR_INDIRECTLY_WRITABLE_HPP
#define VCCC_ITERATOR_INDIRECTLY_WRITABLE_HPP

#include <type_traits>
#include <utility>

#include "vccc/__concepts/dereferenceable.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"
#include "vccc/__type_traits/is_referenceable.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<
    typename Out,
    typename T,
    bool = conjunction<
               dereferenceable<decltype(std::forward<Out>(std::declval<Out&&>()))>,
               is_referencable<iter_reference_t<Out>>
           >::value /* true */
>
struct indirectly_writable_impl_2
    : conjunction<
          std::is_assignable<decltype(*std::declval<Out&&>()), decltype(std::forward<T>(std::declval<T&&>()))>,
          std::is_assignable<decltype(*std::forward<Out>(std::declval<Out&&>())), decltype(std::forward<T>(std::declval<T&&>()))>,
          std::is_assignable<decltype(const_cast<const iter_reference_t<Out>&&>(*std::declval<Out&&>())), decltype(std::forward<T>(std::declval<T&&>()))>,
          std::is_assignable<decltype(const_cast<const iter_reference_t<Out>&&>(*std::forward<Out>(std::declval<Out&&>()))), decltype(std::forward<T>(std::declval<T&&>()))>
      >{};

template<typename Out, typename T>
struct indirectly_writable_impl_2<Out, T, false> : std::false_type {};

template<
    typename Out,
    typename T,
    bool = conjunction<
               dereferenceable<Out>,
               is_referencable<T>,
               has_typename_type<iter_reference<Out>>
           >::value /* true */
>
struct indirectly_writable_impl : indirectly_writable_impl_2<Out, T> {};
template<typename Out, typename T>
struct indirectly_writable_impl<Out, T, false> : std::false_type {};

} // namespace detail

/// @addtogroup iterator
/// @{

/**
@brief specifies that a value can be written to an iterator's referenced object

The concept `%indirectly_writable<Out, T>` specifies the requirements for writing a value whose type and value category
are encoded by `T` into an iterator `Out`'s referenced object.

@sa [std::indirectly_writable](https://en.cppreference.com/w/cpp/iterator/indirectly_writable)
 */
template<typename Out, typename T>
struct indirectly_writable : detail::indirectly_writable_impl<Out, T> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECTLY_WRITABLE_HPP
