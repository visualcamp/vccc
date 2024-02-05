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
# include "vccc/__type_traits/core/INVOKE.hpp"
# include "vccc/__type_traits/is_invocable.hpp"

namespace vccc {
namespace detail {

template<typename R, typename F, typename... Args>
constexpr void invoke_r_impl(std::true_type /* is_void */, F&& f, Args&&... args);

template<typename R, typename F, typename... Args>
constexpr R invoke_r_impl(std::false_type /* is_void */, F&& f, Args&&... args);

} // namespace detail

/**
@addtogroup functional
@{
@addtogroup functional_invoke__func__invoke invoke, inivoke_r
@brief invokes any [Callable](https://en.cppreference.com/w/cpp/named_req/Callable) object with given arguments
@{

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
  return vccc::detail::invoke_r_impl<R>(std::is_void<R>{}, std::forward<F>(f), std::forward<Args>(args)...);
}
/// @}


/// @}
///
namespace detail {

template<typename R, typename F, typename... Args>
constexpr void invoke_r_impl(std::true_type /* is_void */, F&& f, Args&&... args) {
  static_cast<void>(detail::INVOKE(std::forward<F>(f), std::forward<Args>(args)...));
}

template<typename R, typename F, typename... Args>
constexpr R invoke_r_impl(std::false_type /* is_void */, F&& f, Args&&... args) {
  return detail::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
}

} // namespace detail
} // namespace vccc

#endif // VCCC_FUNCTIONAL_INVOKE_HPP
