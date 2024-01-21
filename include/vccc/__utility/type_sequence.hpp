//
// Created by yonggyulee on 1/12/24.
//

#ifndef VCCC_UTILITY_TYPE_SEQUENCE_HPP
#define VCCC_UTILITY_TYPE_SEQUENCE_HPP

#include <cstddef>

namespace vccc {

/// @addtogroup utility
/// @{

// Simple type storage (same with not implemented std::tuple)
template<typename...>
struct type_sequence;

namespace detail {

template<typename S1, typename S2>
struct type_sequence_cat_impl;
template<typename... Ts, typename... Us>
struct type_sequence_cat_impl<type_sequence<Ts...>, type_sequence<Us...>> {
  using type = type_sequence<Ts..., Us...>;
};
template<typename S1, typename S2>
using type_sequence_cat_t = typename type_sequence_cat_impl<S1, S2>::type;

template<typename T, std::size_t>
struct make_type_sequence_impl;

template<typename T, std::size_t N> struct type_sequence_impl;
template<typename T> struct make_type_sequence_impl<T, 0> { using type = type_sequence<>; };
template<typename T> struct make_type_sequence_impl<T, 1> { using type = type_sequence<T>; };
template<typename T> struct make_type_sequence_impl<T, 2> { using type = type_sequence<T, T>; };
template<typename T, std::size_t N>
struct make_type_sequence_impl {
  using type = type_sequence_cat_t<typename make_type_sequence_impl<T, (N / 2)>::type,
                                   typename make_type_sequence_impl<T, N - (N / 2)>::type>;
};

} // namespace detail

template<typename T, std::size_t N>
using make_type_sequence = typename detail::make_type_sequence_impl<T, N>::type;

/// @}

} // namespace vccc

#endif // VCCC_UTILITY_TYPE_SEQUENCE_HPP
