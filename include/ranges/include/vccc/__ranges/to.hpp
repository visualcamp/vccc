//
// Created by yonggyulee on 2024/01/06.
//

#ifndef VCCC_RANGES_TO_HPP_
#define VCCC_RANGES_TO_HPP_

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/range_size_t.hpp"
#include "vccc/__ranges/sized_range.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/disjunction.hpp"
#include "vccc/__type_traits/is_referenceable.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__type_traits/void_t.hpp"


namespace vccc {
namespace ranges {
namespace detail {

template<typename T, typename N, typename = void>
struct has_mf_reserve : std::false_type {};
template<typename T, typename N>
struct has_mf_reserve<T, N, void_t<decltype( std::declval<T&>().reserve(std::declval<N>()) )>> : std::true_type {};

template<typename T, typename N, typename = void>
struct has_mf_capacity : std::false_type {};
template<typename T, typename N>
struct has_mf_capacity<T, N, void_t<decltype( std::declval<T&>().capacity() )>>
    : same_as<decltype(std::declval<T&>().capacity()), N> {};

template<typename T, typename N, typename = void>
struct has_mf_max_size : std::false_type {};
template<typename T, typename N>
struct has_mf_max_size<T, N, void_t<decltype( std::declval<T&>().max_size() )>>
    : same_as<decltype(std::declval<T&>().max_size()), N> {};

template<typename Container, bool = ranges::sized_range<Container>::value /* true */>
struct reservable_container
    : conjunction<
          has_mf_reserve<Container, ranges::range_size_t<Container>>,
          has_mf_reserve<Container, ranges::range_size_t<Container>>,
          has_mf_reserve<Container, ranges::range_size_t<Container>>
      >{};


template<typename T, typename U, typename = void>
struct has_mf_push_back : std::false_type {};
template<typename T, typename U>
struct has_mf_push_back<T, U, void_t<decltype( std::declval<T&>().push_back(std::declval<U>()) )>>
    : std::true_type {};

template<typename T, typename = void>
struct has_mf_end : std::false_type {};
template<typename T>
struct has_mf_end<T, void_t<decltype( std::declval<T&>().end() )>>
    : is_referencable<decltype( std::declval<T&>().end() )> {};

template<typename T, typename U, bool = has_mf_end<T>::value /* false */, typename = void>
struct has_mf_insert : std::false_type {};
template<typename T, typename U>
struct has_mf_insert<T, U, true, void_t<decltype( std::declval<T&>().insert(std::declval<T&>().end(), std::declval<U>()) )>>
    : std::true_type {};

template<typename Container, typename Reference, bool = conjunction<is_referencable<Container>, is_referencable<Reference>>::value>
struct container_insertable
    : disjunction<
          has_mf_push_back<Container, Reference>,
          has_mf_insert<Container, Reference>
      > {};

// template<typename Reference, typename C>
// struct container_inserter()

template<typename Container>
struct reservable_container<Container, false> : std::false_type {};

} // namespace detail

/// @addtogroup ranges
/// @{

// template<typename C, typename R, typename... Args, std::enable_if_t<conjunction<
//     ranges::input_range<R>,
//     negation< ranges::view<C> >
// >::value, int> = 0>
// constexpr C
// to(R&& r, Args&&... args) {
//
// }
//
// template<template<typename...> class C, typename R, typename... Args, std::enable_if_t<conjunction<
//     ranges::input_range<R>
// >::value, int> = 0>
// constexpr auto
// to(R&& r, Args&&... args) {
//
// }

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_TO_HPP_
