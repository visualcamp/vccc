//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_ITERATOR_ITERATOR_TRAITS_FORWARD_DECLARE_HPP_
#define VCCC_ITERATOR_ITERATOR_TRAITS_FORWARD_DECLARE_HPP_

#if __cplusplus >= 202002L
#include <iterator>
#endif
#include <type_traits>

#include "vccc/__type_traits/void_t.hpp"

namespace vccc {

template<typename Iter>
struct cxx20_iterator_traits;

namespace detail {

template<typename T>
struct is_primary_iterator_traits;

// std::iterator_traits

template<typename T> struct is_primary_iterator_traits< std::iterator_traits<T> > : std::true_type {};
template<typename T> struct is_primary_iterator_traits< std::iterator_traits<T*> > : std::false_type {};
#if __cplusplus >= 202002L
template<typename I>
struct is_primary_iterator_traits<std::iterator_traits< std::counted_iterator<I> >> : is_primary_iterator_traits<I> {};
template<typename I, typename S>
struct is_primary_iterator_traits<std::iterator_traits< std::common_iterator<I, S> >> : std::false_type {};
#endif


// vccc::cxx20_iterator_traits

template<typename T> struct is_primary_iterator_traits< cxx20_iterator_traits<T> > : std::true_type {};
template<typename T> struct is_primary_iterator_traits< cxx20_iterator_traits<T*> > : std::false_type {};

#if __cplusplus >= 202002L
template<typename I>
struct is_primary_iterator_traits<cxx20_iterator_traits< std::counted_iterator<I> >> : is_primary_iterator_traits<I> {};
template<typename I, typename S>
struct is_primary_iterator_traits<cxx20_iterator_traits< std::common_iterator<I, S> >> : std::false_type {};
#endif

} // namespace detail

} // namespace vccc

#endif // VCCC_ITERATOR_ITERATOR_TRAITS_FORWARD_DECLARE_HPP_
