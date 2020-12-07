//
// Created by YongGyu Lee on 2020/12/07.
//

#ifndef VCCC_TYPE_TRAITS_BIGGER_TYPE_HPP
#define VCCC_TYPE_TRAITS_BIGGER_TYPE_HPP

#include <type_traits>

namespace vc{

/**
 *  returns bigger type
 */

template<typename ...>
struct bigger_type;

template<typename T>
struct bigger_type<T>;

template<typename T1, typename T2>
struct bigger_type<T1, T2>;

template<typename T1, typename T2, typename ...Ts>
struct bigger_type<T1, T2, Ts...>;

template<typename T>
struct bigger_type<T> {
  static_assert(!std::is_integral<T>::value, "Use signed_bigger_type or unsigned_bigger_type for integer types");
  using type = T;
};

template<typename T1, typename T2>
struct bigger_type<T1, T2> {
  using type = std::conditional_t<sizeof(T1) >= sizeof(T2), typename bigger_type<T1>::type, typename bigger_type<T2>::type>;
};

template<typename T1, typename T2,typename ...Ts>
struct bigger_type<T1, T2, Ts...> {
  using bt = typename bigger_type<T1, T2>::type;
  using type = typename bigger_type<bt, Ts...>::type;
};

template<typename ...Ts>
using bigger_type_t = typename bigger_type<Ts...>::type;


/**
 *  returns signed bigger type of Ts
 *  if unsigned types are provided, smaller unsigned types will be returned
 */
template<typename ...>
struct signed_bigger_type;

template<typename T>
struct signed_bigger_type<T>;

template<typename T1, typename T2>
struct signed_bigger_type<T1, T2>;

template<typename T1, typename T2, typename ...Ts>
struct signed_bigger_type<T1, T2, Ts...>;

template<typename T>
struct signed_bigger_type<T> {
  static_assert(std::is_integral<T>::value, "Use bigger_type for non-integral types");
  using type = T;
};

template<typename T1, typename T2>
struct signed_bigger_type<T1, T2> {
  using type =
  std::conditional_t<sizeof(T1) >= sizeof(T2) && std::is_signed<T1>::value,
                     typename signed_bigger_type<T1>::type,
                     typename signed_bigger_type<T2>::type>;
};

template<typename T1, typename T2, typename ...Ts>
struct signed_bigger_type<T1, T2, Ts...> {
  using bt = typename signed_bigger_type<T1, T2>::type;
  using type = typename signed_bigger_type<bt, Ts...>::type;
};


template<typename ...Ts>
using signed_bigger_type_t = typename signed_bigger_type<Ts...>::type;


/**
 *  returns unsigned bigger type of Ts
 *  if signed types are provided, smaller signed types will be returned
 */

template<typename ...>
struct unsigned_bigger_type;

template<typename T>
struct unsigned_bigger_type<T>;

template<typename T1, typename T2>
struct unsigned_bigger_type<T1, T2>;

template<typename T1, typename T2, typename ...Ts>
struct unsigned_bigger_type<T1, T2, Ts...>;

template<typename T>
struct unsigned_bigger_type<T> {
  static_assert(std::is_integral<T>::value, "Use bigger_type for non-integral types");
  using type = T;
};

template<typename T1, typename T2>
struct unsigned_bigger_type<T1, T2> {
  using type =
  std::conditional_t<sizeof(T1) >= sizeof(T2) && std::is_unsigned<T1>::value,
                     typename unsigned_bigger_type<T1>::type,
                     typename unsigned_bigger_type<T2>::type>;
};

template<typename T1, typename T2, typename ...Ts>
struct unsigned_bigger_type<T1, T2, Ts...> {
  using bt = typename unsigned_bigger_type<T1, T2>::type;
  using type = typename unsigned_bigger_type<bt, Ts...>::type;
};

template<typename ...Ts>
using unsigned_bigger_type_t = typename unsigned_bigger_type<Ts...>::type;

}

#endif //VCCC_TYPE_TRAITS_BIGGER_TYPE_HPP
