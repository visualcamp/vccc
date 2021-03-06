# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_ARE_HPP
# define VCCC_TYPE_TRAITS_ARE_HPP
#
# include <type_traits>

namespace vccc{

//!
//!

/**
@addtogroup type_traits
@{
    @defgroup are are
@}

@addtogroup are
@{

@brief check conditions for multiple template parameters

@code{.cpp}

template<typename ...Args>
void foo() {
    std::cout << std::boolalpha;
    std::cout << vccc::are_<std::is_integral<Args>...>{} << std::endl;
}

foo<int, int>();
foo<int, int, double>();

@endcode

@tparam Conds...
 */

template<typename... Conds>
struct are_ : std::true_type {};


//! @cond ignored
template<typename Cond, typename... Conds>
struct are_<Cond, Conds...> : std::conditional<Cond::value, are_<Conds...>, std::false_type>::type {};
//! @endcond

template<typename ...Conds>
using are_t = typename are_<Conds...>::type;

template<typename ...Conds>
constexpr bool are_v = are_<Conds...>::value;


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

//! @} are

}

# endif //VCCC_TYPE_ARE_HPP_
