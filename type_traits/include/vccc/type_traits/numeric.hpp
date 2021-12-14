# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_NUMERIC_HPP
# define VCCC_TYPE_TRAITS_NUMERIC_HPP
#
# include <type_traits>

namespace vccc{

/**
@addtogroup type_traits
@{

@addtogroup multiples_of multiples_of
@{
*/

/** @brief check if A is multiples of B

@tparam T
@tparam A
@tparam B

@sa is_odd
@sa is_even
 */

template<typename T, T A, T B>
struct multiples_of : std::integral_constant<bool, A%B==0> {};

template<typename T, T A, T B>
using multiples_of_t = typename multiples_of<T, A, B>::type;

template<typename T, T A, T B>
constexpr bool multiples_of_v = multiples_of<T, A, B>::value;

//! @} multiples_of

/**
@addtogroup is_odd is_odd
@{
*/

/**
@brief check if a value is odd

@tparam T
@tparam A

@sa is_even
*/
template<typename T, T A>
struct is_odd : std::integral_constant<bool, A&1> {
  static_assert(A >= 0, "value must be natural number");
};

template<typename T, T A>
using is_odd_t = typename is_odd<T, A>::type;

template<typename T, T A>
constexpr bool is_odd_v = is_odd<T, A>::value;

//! @} is_odd

/**
@addtogroup is_even is_even
@{
*/

/**
@brief check if a value is even

@tparam T
@tparam A

@sa is_odd
*/

template<typename T, T A>
struct is_even : std::integral_constant<bool, (A&1) != 1> {
  static_assert(A>=0, "value must be natural number");
};

template<typename T, T A>
using is_even_t = typename is_even<T, A>::type;

template<typename T, T A>
constexpr bool is_even_v = is_even<T, A>::value;

//! @} is_even

/**
@addtogroup type_traits_static_max static_max
@{

@brief Get max value in compile-time
@sa static_min
@sa static_diff
*/

template<typename T, T ...>
struct static_max;

template<typename T, T v>
struct static_max<T, v> : std::integral_constant<T, v> {};

template<typename T, T v1, T v2, T ...v3>
struct static_max<T, v1, v2, v3...>
    : std::conditional_t<(v1 > v2), static_max<T, v1, v3...>, static_max<T, v2, v3...>> {};

//! @} type_traits_static_max

/**
@addtogroup type_traits_static_min static_min
@{

@brief Get min value in compile-time
@sa static_max
@sa static_diff
*/

template<typename T, T ...>
struct static_min;

template<typename T, T v>
struct static_min<T, v> : integral_constant<T, v> {};

template<typename T, T v1, T v2, T ...v3>
struct static_min<T, v1, v2, v3...>
    : std::conditional_t<(v1 < v2), static_max<T, v1, v3...>, static_max<T, v2, v3...>> {};


/**

@brief Get abs(v1 - v2) in compile-time
@sa static_min
@sa static_max
*/
template<typename T, T v1, T v2>
struct static_diff {
  static constexpr T value = static_max<T, v1, v2>::value - static_min<T, v1, v2>::value;
};

//! @} type_traits




}

# endif //VCCC_TYPE_TRAITS_NUMERIC_HPP
