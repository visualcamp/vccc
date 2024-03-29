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

#include "vccc/__type_traits/bool_constant.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"
#include "vccc/__type_traits/void_t.hpp"

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

template<typename T, std::size_t X, bool v = /* true */ has_tuple_size<T>::value>
struct tuple_size_greater_than : bool_constant<(std::tuple_size<T>::value > X)> {};

template<typename T, std::size_t X>
struct tuple_size_greater_than<T, X, false> : std::false_type {};

template<typename T, std::size_t X, bool v = /* true */ has_tuple_size<T>::value>
struct tuple_size_less_than : bool_constant<(std::tuple_size<T>::value < X)> {};

template<typename T, std::size_t X>
struct tuple_size_less_than<T, X, false> : std::false_type {};

template<typename T>
struct tuple_like_uncvref : std::false_type {};


template<typename T, std::size_t N>
struct tuple_like_uncvref<std::array<T, N>> : std::true_type {};

template<typename T, typename U>
struct tuple_like_uncvref<std::pair<T, U>> : std::true_type {};

template<typename ...T>
struct tuple_like_uncvref<std::tuple<T...>> : std::true_type {};

} // namespace internal

/// @addtogroup tuple
/// @{

/**
 * @brief specifies that a type implemented the tuple protocol
 *
 * @code
template<typename T>
struct vccc::pair_like< T >
 * @endcode
 *
 * 1) A type T models and satisfies the concept tuple-like if std::remove_cvref_t<T> is a specialization of
 * - `std::array`,
 * - `std::pair`,
 * - `std::tuple`, or
 * - <strike>`std::ranges::subrange`</strike>.
 *
 * 2) \a pair-like objects are \a tuple-like objects with exactly 2 elements.
 *
 * @par Notes
 * @parblock
 * tuple-like types implement the tuple protocol, i.e., such types can be
 * used with `std::get`, `std::tuple_element` and `std::tuple_size`.
 *
 * Elements of tuple-like types can be bound with structured binding.
 * @endparblock
 *
 * @sa pair_like: @copybrief pair_like
 */
template<typename T>
struct tuple_like : internal::tuple_like_uncvref<remove_cvref_t<T>> {};


/**
 * @brief check if tuple-like objects with exactly N elements.
 *
 * @par See Also
 * @ref tuple_like "\a tuple-like ": @copybrief tuple_like
 */
template<typename T, std::size_t N>
struct sized_tuple_like
    : conjunction<
          tuple_like<T>,
          internal::tuple_size_equal_to<T, N>
      > {};


/**
 * @brief pair-like objects are tuple-like objects with exactly 2 elements.
 *
 * @par See Also
 * @ref tuple_like "\a tuple-like ": @copybrief tuple_like
 */
template<typename T>
using pair_like = sized_tuple_like<T, 2>;

/// @}

} // namespace vccc

#endif // VCCC_TUPLE_TUPLE_LIKE_HPP
