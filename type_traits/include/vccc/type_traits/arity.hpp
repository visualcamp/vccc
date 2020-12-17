# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_ARITY_HPP
# define VCCC_TYPE_TRAITS_ARITY_HPP
#
# include <type_traits>

namespace vccc{

template <typename T>
struct arity : arity<decltype(&T::operator())> {};

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

template<typename T>
using arity_t = typename arity<T>::type;

template<typename T>
constexpr unsigned arity_v = arity<T>::value;

}

#endif //VCCC_TYPE_TRAITS_ARITY_HPP
