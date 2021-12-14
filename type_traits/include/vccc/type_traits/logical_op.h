# /*
#  * Created by YongGyu Lee on 2021/09/03.
#  */
#
# ifndef VCCC_TYPE_TRAITS_LOGICAL_OP_H_
# define VCCC_TYPE_TRAITS_LOGICAL_OP_H_
#
# include "vccc/type_traits/integral_constant.h"

namespace vccc {

/**
 * conjunction
 * @tparam ...B
 */
template<typename ...B> struct conjunction;
template<> struct conjunction<> : std::true_type {};
template<typename B1> struct conjunction<B1> : B1 {};
template<typename B1, typename ...BN>
struct conjunction<B1, BN...> : std::conditional_t<bool(B1::value), conjunction<BN...>, B1> {};

template<typename ...B>
constexpr bool conjunction_v = conjunction<B...>::value;


/**
 * disjunction
 * @tparam ...B
 */
template<typename ...B> struct disjunction;
template<> struct disjunction<> : std::false_type {};
template<typename B1> struct disjunction<B1> : B1 {};
template<typename B1, typename ...BN>
struct disjunction<B1, BN...> : std::conditional_t<bool(B1::value), B1, disjunction<BN...>> {};

template<typename ...B>
constexpr bool disjunction_v = disjunction<B...>::value;


/**
 * negation
 * @tparam B
 */
template<typename B> struct negation : bool_constant<!bool(B::value)> {};
template<typename B>
constexpr bool negation_v = negation<B>::value;

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_LOGICAL_OP_H_
