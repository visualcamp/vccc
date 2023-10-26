//
// Created by yonggyulee on 2023/10/27.
//

#ifndef VCCC_TUPLE_MAKE_FROM_TUPLE_HPP
#define VCCC_TUPLE_MAKE_FROM_TUPLE_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

namespace vccc {

/// @addtogroup tuple
/// @{

namespace internal {

template<typename T, typename Tuple, std::size_t... I>
constexpr T make_from_tuple_impl(Tuple&& t, std::index_sequence<I...>) {
  return T(std::get<I>(std::forward<Tuple>(t))...);
}

template<typename T, typename Tuple, typename I>
struct is_constructible_from_tuple_impl;

template<typename T, typename Tuple, std::size_t... I>
struct is_constructible_from_tuple_impl<T, Tuple, std::index_sequence<I...>>
    : std::is_constructible<T, decltype(std::get<I>(std::declval<Tuple>()))...> {};

template<typename T, typename Tuple>
struct is_constructible_from_tuple
    : is_constructible_from_tuple_impl<T,
                                       Tuple,
                                       std::make_index_sequence<
                                           std::tuple_size<std::remove_reference_t<Tuple>>::value>> {};

} // namespace internal

// TODO(Tony): Constraint on Tuple? (C++23)

/**
 * @brief construct an object with a tuple of arguments
 *
 * Construct an object of type T, using the elements of the tuple `t` as the arguments to the constructor.
 *
 * @tparam T
 * @tparam Tuple
 * @param t tuple whose elements to be used as arguments to the constructor of T
 * @return The constructed T object or reference.
 */
template<typename T, typename Tuple>
constexpr std::enable_if_t<internal::is_constructible_from_tuple<T, Tuple>::value, T>
make_from_tuple(Tuple&& t) {
  return internal::make_from_tuple_impl<T>(
      std::forward<Tuple>(t),
      std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

/// @} tuple

} // namespace vccc

#endif // VCCC_TUPLE_MAKE_FROM_TUPLE_HPP
