//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_COPY_CONSTRUCTIBLE_HPP_
#define VCCC_CONCEPTS_COPY_CONSTRUCTIBLE_HPP_

#include "vccc/concepts/move_constructible.hpp"
#include "vccc/concepts/constructible_from.hpp"
#include "vccc/concepts/convertible_to.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace detail {

template<typename T, bool = is_referencable<T>::value>
struct copy_constructible_impl : std::false_type {};

template<typename T>
struct copy_constructible_impl<T, true>
    : conjunction<
        move_constructible<T>,
        constructible_from<T, T&>, convertible_to<T&, T>,
        constructible_from<T, const T&>, convertible_to<const T&, T>,
        constructible_from<T, const T>, convertible_to<const T, T>
      > {};

} // namespace detail

/// @addtogroup concepts
/// @{

template<typename T>
struct copy_constructible : detail::copy_constructible_impl<T> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_COPY_CONSTRUCTIBLE_HPP_
