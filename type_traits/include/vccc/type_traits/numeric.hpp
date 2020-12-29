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
    @defgroup multiples_of multiples_of
@}

@addtogroup multiples_of
@{ */

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

/** @addtogroup type_traits
@{
    @defgroup is_odd is_odd
@}

@addtogroup is_odd
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
@addtogroup type_traits
@{
    @defgroup is_even is_even
@}

@addtogroup is_even
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

//! @}

}

# endif //VCCC_TYPE_TRAITS_NUMERIC_HPP
