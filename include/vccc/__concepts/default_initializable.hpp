//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_DEFAULT_INITIALIZABLE_HPP_
#define VCCC_CONCEPTS_DEFAULT_INITIALIZABLE_HPP_

#include "vccc/__concepts/constructible_from.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename T, typename = void>
struct is_direct_list_initializable : std::false_type {};
template<typename T>
struct is_direct_list_initializable<T, void_t<decltype(T{})>> : std::true_type {};

template<typename T, typename = void>
struct is_default_initializable : std::false_type {};
template<typename T>
struct is_default_initializable<T, void_t<decltype(::new T)>> : std::true_type {};

} // namespace detail

/// @addtogroup concepts
/// @{

/**
@brief specifies that an object of a type can be default constructed

@code{.cpp}
template<typename T, bool = is_referencable<T>::value>
struct copy_constructible_impl
    : conjunction<
        move_constructible<T>,
        constructible_from<T, T&>, convertible_to<T&, T>,
        constructible_from<T, const T&>, convertible_to<const T&, T>,
        constructible_from<T, const T>, convertible_to<const T, T>
      > {};
@endcode

The `%default_initializable` concept checks whether variables of type `T` can be

- value-initialized (`T()` is well-formed);
- direct-list-initialized from an empty initializer list (`T{}` is well-formed); and
- default-initialized (`T t;` is well-formed).

Access checking is performed as if in a context unrelated to `T`. Only the validity of the immediate context of the
variable initialization is considered.

@sa [std::default_initializable](https://en.cppreference.com/w/cpp/concepts/default_initializable)
@sa constructible_from
 */
template<typename T>
struct default_initializable
    : conjunction<
        constructible_from<T>,
        detail::is_direct_list_initializable<T>,
        detail::is_default_initializable<T>
      > {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_DEFAULT_INITIALIZABLE_HPP_
