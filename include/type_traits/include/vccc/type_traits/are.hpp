# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_ARE_HPP
# define VCCC_TYPE_TRAITS_ARE_HPP
#
# include <type_traits>
#
# include "vccc/type_traits/conjunction.hpp"

namespace vccc {

/**
@addtogroup type_traits
@{
    @defgroup are are
@}
*/

//! @addtogroup are
//! @{

/** are_arithmetic */

template<typename ...Ts>
struct are_arithmetic : conjunction<std::is_arithmetic<Ts>...> {};

template<typename ...Ts>
using are_arithmetic_t = typename are_arithmetic<Ts...>::type;


/** are_integral */

template<typename ...Ts>
struct are_integral : conjunction<std::is_integral<Ts>...> {};

template<typename ...Ts>
using are_integral_t = typename are_integral<Ts...>::type;


/** are_floating_point */

template<typename ...Ts>
struct are_floating_point : conjunction<std::is_floating_point<Ts>...> {};

template<typename ...Ts>
using are_floating_point_t = typename are_floating_point<Ts...>::type;


/** are_scalar */

template<typename ...Ts>
struct are_scalar : conjunction<std::is_scalar<Ts>...> {};

template<typename ...Ts>
using are_scalar_t = typename are_scalar<Ts...>::type;

//! @} are

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_ARE_HPP
