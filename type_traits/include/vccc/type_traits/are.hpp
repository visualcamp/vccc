# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_ARE_HPP
# define VCCC_TYPE_TRAITS_ARE_HPP
#
# include <type_traits>

namespace vccc{


/** are */

template<typename... Conds>
struct are_ : std::true_type {};

template<typename Cond, typename... Conds>
struct are_<Cond, Conds...> : std::conditional<Cond::value, are_<Conds...>, std::false_type>::type {};

/*

template<typename ...>
struct are_ : public std::true_type{};

template<typename C0>
struct are_<C0>
    : public std::integral_constant<bool, C0::value> {};

template<typename C0, typename C1>
struct are_<C0, C1>
    : public std::integral_constant<bool, C0::value && C1::value> {};

template<typename C0, typename C1, typename C2>
struct are_<C0, C1, C2>
    : public std::integral_constant<bool, C0::value && C1::value &&
                                          C2::value> {};

template<typename C0, typename C1, typename C2, typename C3>
struct are_<C0, C1, C2, C3>
    : public std::integral_constant<bool, C0::value && C1::value &&
                                          C2::value && C3::value> {};

template<typename C0, typename C1, typename C2, typename C3,
         typename C4>
struct are_<C0, C1, C2, C3, C4>
    : public std::integral_constant<bool, C0::value && C1::value &&
                                          C2::value && C3::value &&
                                          C4::value> {};

template<typename C0, typename C1, typename C2, typename C3,
         typename C4, typename C5>
struct are_<C0, C1, C2, C3, C4, C5>
    : public std::integral_constant<bool, C0::value && C1::value &&
                                          C2::value && C3::value &&
                                          C4::value && C5::value> {};

template<typename C0, typename C1, typename C2, typename C3,
         typename C4, typename C5, typename C6>
struct are_<C0, C1, C2, C3, C4, C5, C6>
    : public std::integral_constant<bool, C0::value && C1::value &&
                                          C2::value && C3::value &&
                                          C4::value && C5::value &&
                                          C6::value> {};

template<typename C0, typename C1, typename C2, typename C3,
         typename C4, typename C5, typename C6, typename C7>
struct are_<C0, C1, C2, C3, C4, C5, C6, C7>
    : public std::integral_constant<bool, C0::value && C1::value &&
                                          C2::value && C3::value &&
                                          C4::value && C5::value &&
                                          C6::value && C7::value> {};

template<typename C0, typename C1, typename C2, typename C3,
         typename C4, typename C5, typename C6, typename C7,
         typename ...Cs>
struct are_<C0, C1, C2, C3, C4, C5, C6, C7, Cs...>
    : public std::integral_constant<bool, C0::value && C1::value &&
                                          C2::value && C3::value &&
                                          C4::value && C5::value &&
                                          C6::value && C7::value &&
                                          are_<Cs...>::value> {};
 */


template<typename ...Conds>
using are_t = typename are_<Conds...>::type;

template<typename ...Conds>
constexpr auto are_v = are_<Conds...>::value;


/**
 *      are_ helper of std::is_...<>
 */


/** are_arithmetic */

template<typename ...Ts>
struct are_arithmetic : are_<std::is_arithmetic<Ts>...> {};

template<typename ...Ts>
using are_arithmetic_t = typename are_arithmetic<Ts...>::type;

template<typename ...Ts>
constexpr auto are_arithmetic_v = are_arithmetic<Ts...>::value;


/** are_integral */

template<typename ...Ts>
struct are_integral : are_<std::is_integral<Ts>...> {};

template<typename ...Ts>
using are_integral_t = typename are_integral<Ts...>::type;

template<typename ...Ts>
constexpr auto are_integral_v = are_integral<Ts...>::value;


/** are_floating_point */

template<typename ...Ts>
struct are_floating_point : are_<std::is_floating_point<Ts>...> {};

template<typename ...Ts>
using are_floating_point_t = typename are_floating_point<Ts...>::type;

template<typename ...Ts>
constexpr auto are_floating_point_v = are_<std::is_floating_point<Ts>...>::value;


/** are_scalar */

template<typename ...Ts>
struct are_scalar : are_<std::is_scalar<Ts>...> {};

template<typename ...Ts>
using are_scalar_t = typename are_scalar<Ts...>::type;

template<typename ...Ts>
constexpr auto are_scalar_v = are_scalar<Ts...>::value;

}

# endif //VCCC_TYPE_ARE_HPP_
