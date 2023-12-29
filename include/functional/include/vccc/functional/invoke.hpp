# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
#ifndef VCCC_FUNCTIONAL_INVOKE_HPP
#define VCCC_FUNCTIONAL_INVOKE_HPP
#
# include <type_traits>
# include <utility>
#
# include "vccc/type_traits/core/INVOKE.hpp"
# include "vccc/type_traits/empty.hpp"
# include "vccc/type_traits/is_invocable.hpp"
# include "vccc/type_traits/type_identity.hpp"

namespace vccc {


//! @addtogroup functional
//! @{

/**
@brief deduces the result type of invoking a callable object with a set of arguments

Deduces the return type of an `INVOKE` expression at compile time.
 */
template<typename F, typename ...Args>
struct invoke_result
    : std::conditional_t<
          detail::is_invocable_r_impl<void, F, Args...>::invocable::value,
              type_identity<typename detail::is_invocable_r_impl<void, F, Args...>::test_return_type>,
          empty> {};

template<typename F, typename ...Args>
using invoke_result_t = typename invoke_result<F, Args...>::type;

/// @addtogroup functional_invoke__func__invoke invoke, inivoke_r
/// @brief invokes any [Callable](https://en.cppreference.com/w/cpp/named_req/Callable) object with given arguments
/// @{

/**
Invoke the [Callable](https://en.cppreference.com/w/cpp/named_req/Callable) object `f` with the parameters `args`
as by `INVOKE(std::forward<F>(f), std::forward<Args>(args)...)`. This overload participates in overload resolution
only if `vccc::is_invocable<F, Args...>::value` is `true`.
 */
template<typename F, typename ...Args>
constexpr invoke_result_t<F, Args...>
invoke(F&& f, Args&&... args)
noexcept(is_nothrow_invocable<F, Args...>::value)
{
  return detail::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
}

/**
Invoke the [Callable](https://en.cppreference.com/w/cpp/named_req/Callable) object `f` with the parameters args as by
`INVOKE<R>(std::forward<F>(f), std::forward<Args>(args)...)`. This overload participates in overload resolution
only if `vccc::is_invocable_r<R, F, Args...>::value` is `true`.
 */
template<typename R, typename F, typename ...Args>
constexpr std::enable_if_t<is_invocable_r<R, F, Args...>::value, R>
invoke_r(F&& f, Args&&... args)
noexcept(is_nothrow_invocable_r<R, F, Args...>::value)
{
  return detail::INVOKE<R>(std::forward<F>(f), std::forward<Args>(args)...);
}
/// @}


/// @}

} // namespace vccc

#endif // VCCC_FUNCTIONAL_INVOKE_HPP
