# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_LOSSLESS_TYPE_HPP
# define VCCC_TYPE_TRAITS_LOSSLESS_TYPE_HPP
#
# include "vccc/type_traits/are.hpp"
# include "vccc/type_traits/bigger_type.hpp"
# include "vccc/type_traits/detail/lossless_type_int_division.hpp"

namespace vccc{


//! @addtogroup type_traits
//! @{

// TODO: change double to biggest among input floating_point type
template<typename ...Types>
using decay_if_float_t = typename std::conditional_t<are_<std::is_same<Types, float>...>::value, float, double>;


/**
 * lossless_type_add
 */

template<typename...>struct lossless_type_add;

template<typename ...Ts>
using lossless_type_add_t = typename lossless_type_add<Ts...>::type;

//! @cond ignored

template<typename T>
struct lossless_type_add<T> {
  static_assert(std::is_arithmetic<T>::value, "Invalid type in lossless_type_add<>");
  using type = T;
};

template<typename T1, typename T2>
struct lossless_type_add<T1, T2> {
  using type = decltype(static_cast<lossless_type_add_t<T1>>(0) +
                        static_cast<lossless_type_add_t<T2>>(0));
};

template<typename T1, typename T2, typename ...Ts>
struct lossless_type_add<T1, T2, Ts...> {
  using type = decltype(static_cast<lossless_type_add_t<T1, T2>>(0) +
                        static_cast<lossless_type_add_t<Ts...>>(0));
};

//! @endcond


/**
 * lossless_type_mul
 */

template<typename...>struct lossless_type_mul;

template<typename ...Ts>
using lossless_type_mul_t = typename lossless_type_mul<Ts...>::type;

//! @cond ignored
template<typename T>
struct lossless_type_mul<T> {
  static_assert(std::is_arithmetic<T>::value, "Invalid type in lossless_type_mul<>");
  using type = T;
};

template<typename T1, typename T2>
struct lossless_type_mul<T1, T2> {
  using type =
      decltype(static_cast<lossless_type_mul_t<T1>>(0) *
               static_cast<lossless_type_mul_t<T2>>(0));
};

template<typename T1, typename T2, typename ...Ts>
struct lossless_type_mul<T1, T2, Ts...> {
  using type =
      decltype(static_cast<lossless_type_mul_t<T1, T2>>(0) *
               static_cast<lossless_type_mul_t<Ts...>>(0));
};

//! @endcond


/**
 * lossless_type_div
 */


template<typename...> struct lossless_type_div;

template<typename ...Ts>
using lossless_type_div_t = typename lossless_type_div<Ts...>::type;


//! @cond ignored
template<typename T>
struct lossless_type_div<T> {
  static_assert(std::is_arithmetic<T>::value, "Invalid type in lossless_type_div<>");
  using type = T;
};

template<typename T1, typename T2>
struct lossless_type_div<T1, T2> {
  using type =
      std::conditional_t<are_integral_v<T1, T2>,
          detail::lossless_type_int_division_t<T1, T2>,
          decltype(static_cast<lossless_type_div_t<T1>>(0) /
                   static_cast<lossless_type_div_t<T2>>(0))>;
};

template<typename T1, typename T2, typename ...Ts>
struct lossless_type_div<T1, T2, Ts...> {
  using type =
      decltype(static_cast<lossless_type_div_t<T1, T2>>(0) /
               static_cast<lossless_type_div_t<Ts...>>(0));
};

//! @endcond

//! @} type_traits

}
# endif //VCCC_TYPE_TRAITS_LOSSLESS_TYPE_HPP
