//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCCC_UTILITY_AT_HPP
#define VCCC_UTILITY_AT_HPP

#include <cstddef>
#include <tuple>
#include <iterator>

#include "vccc/type_traits.hpp"

namespace vc{


/** tuple-like */

template<std::size_t i, typename T,
    VCCC_REQUIRE(i < std::tuple_size<T>::value)>
constexpr
decltype(auto) at (T& tuple) {
  return std::get<i>(tuple);
}

template<std::size_t i, typename T,
    VCCC_REQUIRE(i < std::tuple_size<T>::value)>
constexpr
decltype(auto) at (const T& tuple) {
  return std::get<i>(tuple);
}


/** container */

template<std::size_t i, typename Container,
VCCC_REQUIRE(is_container_v<Container>)>
decltype(auto) at(Container& container) {
  BOUNDS_ASSERT(i, container.size());
  return *std::next(std::begin(container), i);
}

template<std::size_t i, typename Container,
VCCC_REQUIRE(is_container_v<Container>)>
decltype(auto) at(const Container& container) {
  BOUNDS_ASSERT(i, container.size());
  return *std::next(std::begin(container), i);
}


/** std::array */

template<std::size_t i, typename T, int n,
    VCCC_REQUIRE(i < n)>
constexpr
decltype(auto) at(std::array<T, n>& arr) {
  return arr[i];
}

template<std::size_t i, typename T, int n,
    VCCC_REQUIRE(i < n)>
constexpr
decltype(auto) at(const std::array<T, n>& arr) {
  return arr[i];
}


/** variadic */

template<std::size_t i, typename ...Args,
    VCCC_REQUIRE(i < sizeof...(Args))>
constexpr
decltype(auto) variadic_at(Args&&... args){
  return std::get<i>(std::forward_as_tuple(std::forward<Args>(args)...));
}

}

#endif //VCCC_UTILITY_AT_HPP
