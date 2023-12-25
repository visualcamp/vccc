//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_RANGES_ITERATOR_T_HPP_
#define VCCC_RANGES_ITERATOR_T_HPP_

#include <type_traits>

#include "vccc/ranges/begin.hpp"
#include "vccc/type_traits/is_referenceable.hpp"

namespace vccc {
namespace ranges {

template<typename T, bool = is_referencable<T>::value, typename = void>
struct iterator {};

template<typename T>
struct iterator<T, true, void_t<decltype( ranges::begin(std::declval<T&>()) )>> {
  using type = decltype(ranges::begin(std::declval<T&>()));
};

template<typename T>
using iterator_t = typename ranges::iterator<T>::type;

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_ITERATOR_T_HPP_
