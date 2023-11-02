# /*
#  * Created by YongGyu Lee on 2021/09/03.
#  */
#
# ifndef VCCC_TYPE_TRAITS_DISJUNCTION_HPP
# define VCCC_TYPE_TRAITS_DISJUNCTION_HPP
#
# include <type_traits>

namespace vccc {

//! @addtogroup type_traits
//! @{

/**
 * disjunction
 * @tparam ...B
 */
template<typename ...B>
struct disjunction;

//! \cond ignored
template<>
struct disjunction<> : std::false_type {};

template<typename B1>
struct disjunction<B1> : B1 {};

template<typename B1, typename ...BN>
struct disjunction<B1, BN...> : std::conditional_t<bool(B1::value), B1, disjunction<BN...>> {};
//! \endcond

//! @} type_traits

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_DISJUNCTION_HPP
