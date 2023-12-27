//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_DEREFERENCEABLE_HPP_
#define VCCC_CONCEPTS_DEREFERENCEABLE_HPP_

#include <type_traits>

#include "vccc/type_traits/is_referenceable.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename T, typename = void>
struct dereferenceable_impl_3 : std::false_type {};

template<typename T>
struct dereferenceable_impl_3<T, void_t<decltype(*std::declval<T&>())>>
    : is_referencable<decltype(*std::declval<T&>())> {};

// Dereferencing a void* is illegal, but some compilers compiles these expression in unevaluated context.
// Omitting dereferenceable_impl_2 does not change the behavior of dereferenceable, but it is to remove warning
template<typename T, bool = std::is_void<std::remove_pointer_t<std::remove_reference_t<T>>>::value /* false */>
struct dereferenceable_impl_2 : dereferenceable_impl_3<T> {};

template<typename T>
struct dereferenceable_impl_2<T, true> : std::false_type {};

template<typename T, bool = is_referencable<T>::value /* true */>
struct dereferenceable_impl_1 : dereferenceable_impl_2<T> {};

template<typename T>
struct dereferenceable_impl_1<T, false> : std::false_type {};

} // namespace detail


/// @addtogroup concepts
/// @{

/// @brief specifies that an object of a type can be dereferenced
template<typename T>
struct dereferenceable : detail::dereferenceable_impl_1<T> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_DEREFERENCEABLE_HPP_
