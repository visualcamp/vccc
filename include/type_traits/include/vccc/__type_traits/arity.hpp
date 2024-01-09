# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_ARITY_HPP
# define VCCC_TYPE_TRAITS_ARITY_HPP
#
# include <type_traits>

namespace vccc {

/**
 * @addtogroup type_traits
 * @{
 * @defgroup type_traits_arity__class arity
 * @brief get function arity
 * @}

@addtogroup type_traits_arity__class
@{
*/


/**
@brief get parameter count of a given function

using on non-function types will cause syntax failure.
@code{.cpp}
    auto f = [](int a, int b){
        return a+b;
    }

    auto n = arity<decltype(f)>::value;
@endcode

@tparam T function type
 */
template <typename T>
struct arity : arity<decltype(&T::operator())> {};


//! @cond ignored
template <typename R, typename... Args>
struct arity<R(*)(Args...)> : std::integral_constant<unsigned, sizeof...(Args)> {};

// Possibly add specialization for variadic functions
// Member functions:
template <typename R, typename C, typename... Args>
struct arity<R(C::*)(Args...)> :
    std::integral_constant<unsigned, sizeof...(Args)> {};

template <typename R, typename C, typename... Args>
struct arity<R(C::*)(Args...) const> :
    std::integral_constant<unsigned, sizeof...(Args)> {};
//! @endcond

template<typename T>
using arity_t = typename arity<T>::type;

//! @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_ARITY_HPP
