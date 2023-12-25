//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_ITERATOR_ITERATOR_TRAITS_FORWARD_DECLARE_HPP_
#define VCCC_ITERATOR_ITERATOR_TRAITS_FORWARD_DECLARE_HPP_

#include <type_traits>

#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename T, typename = void>
struct is_specialized_iterator_traits : std::false_type {};
template<typename T>
struct is_specialized_iterator_traits<T, void_t<typename T::iterator_concept>> : std::true_type {};

} // namespace detail

template<typename Iter>
struct cxx20_iterator_traits;

} // namespace vccc

#endif // VCCC_ITERATOR_ITERATOR_TRAITS_FORWARD_DECLARE_HPP_
