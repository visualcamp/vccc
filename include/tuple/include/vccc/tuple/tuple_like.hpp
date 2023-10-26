//
// Created by yonggyulee on 2023/10/27.
//

#ifndef VCCC_TUPLE_TUPLE_LIKE_HPP
#define VCCC_TUPLE_TUPLE_LIKE_HPP

#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vccc/type_traits/bool_constant.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/remove_cvref.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace internal {

template<typename T, typename = void>
struct has_tuple_size : std::false_type {};

template<typename T>
struct has_tuple_size<T, void_t<std::tuple_size<T>>> : std::true_type {};

template<typename T, std::size_t X, bool v = /* true */ has_tuple_size<T>::value>
struct tuple_size_equal_to : bool_constant<std::tuple_size<T>::value == X> {};

template<typename T, std::size_t X>
struct tuple_size_equal_to<T, X, false> : std::false_type {};

template<typename T>
struct is_tuple_like_uncvref : std::false_type {};

template<typename T>
struct is_tuple_like : is_tuple_like_uncvref<remove_cvref_t<T>> {};

template<typename T>
struct is_pair_like
    : conjunction<is_tuple_like<T>,
                  tuple_size_equal_to<T, 2>> {};


template<typename T, std::size_t N>
struct is_tuple_like_uncvref<std::array<T, N>> : std::true_type {};

template<typename T, typename U>
struct is_tuple_like_uncvref<std::pair<T, U>> : std::true_type {};

template<typename ...T>
struct is_tuple_like_uncvref<std::tuple<T...>> : std::true_type {};

} // namespace internal
} // namespace vccc

#endif // VCCC_TUPLE_TUPLE_LIKE_HPP
