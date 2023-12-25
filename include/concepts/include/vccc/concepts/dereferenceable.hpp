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

template<typename T, bool = is_referencable<T>::value, typename = void>
struct dereferenceable_impl : std::false_type {};

template<typename T>
struct dereferenceable_impl<T, true, void_t<decltype(*std::declval<T&>())>>
    : is_referencable<decltype(*std::declval<T&>())> {};

} // namespace detail


/// @addtogroup concepts
/// @{

template<typename T>
struct dereferenceable : detail::dereferenceable_impl<T> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_DEREFERENCEABLE_HPP_
