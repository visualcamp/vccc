//
// Created by yonggyulee on 2023/12/29.
//

#ifndef VCCC_TYPE_TRAITS_IS_INVOCABLE_HPP
#define VCCC_TYPE_TRAITS_IS_INVOCABLE_HPP

#include <type_traits>

#include "vccc/type_traits/core/INVOKE.hpp"
#include "vccc/type_traits/bool_constant.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/disjunction.hpp"
#include "vccc/type_traits/empty.hpp"
#include "vccc/type_traits/is_complete.hpp"
#include "vccc/type_traits/is_unbounded_array.hpp"
#include "vccc/type_traits/negation.hpp"

namespace vccc {
namespace detail {

template<typename T>
struct satisfies_invocable_type : disjunction<is_complete<T>, std::is_void<T>, is_unbounded_array<T>> {};

template<typename R, typename F, typename ...Args>
struct is_invocable_r_impl {
 // TODO: handle requirements
 //  static_assert(satisfies_invocable_type<R>::value, "invocable: Return type does not meet the requirements");
 //  static_assert(satisfies_invocable_type<F>::value, "invocable: Callable type does not meet the requirements");
 //  static_assert(conjunction<satisfies_invocable_type<Args>...>::value,
 //                "invocable: Argument types do not meet the requirements");
 private:
 template<typename F2, typename ...Args2>
 static auto test(int)
 noexcept(noexcept(detail::INVOKE(std::declval<F2>(), std::declval<Args2>()...)))
       -> decltype(detail::INVOKE(std::declval<F2>(), std::declval<Args2>()...));
 template<typename F2, typename ...Args2>
 static auto test(...) -> empty;

 public:
 using test_return_type = decltype(test<F, Args...>(0));
 using convertible = disjunction<std::is_void<R>, std::is_convertible<test_return_type, R>>;
 using invocable = conjunction<negation<std::is_same<test_return_type, empty>>, convertible>;
 using nothrow_invocable = conjunction<invocable, bool_constant<noexcept(test<F, Args...>(0))>>;
};

} // namespace detail
/**
@addtogroup type_traits
@{
@addtogroup type_traits_is_invocable__class__Type_relationships is_invocable, is_invocable_r, is_nothrow_invocable, is_nothrow_invocable_r
@brief checks if a type can be invoked (as if by `vccc::invoke`) with the given argument types
@{
 */

/// @brief Determines whether `INVOKE(std::declval<Fn>(), std::declval<ArgTypes>()...)` is well formed when treated as
/// anunevaluated operand.
template<typename F, typename ...Args>
struct is_invocable : detail::is_invocable_r_impl<void, F, Args...>::invocable {};



/// @brief Determines whether `INVOKE<R>(std::declval<Fn>(), std::declval<ArgTypes>()...)` is well formed when treated
/// as an unevaluated operand.
template<typename R, typename F, typename ...Args>
struct is_invocable_r : detail::is_invocable_r_impl<R, F, Args...>::invocable {};



/// @brief Determines whether `INVOKE(std::declval<Fn>(), std::declval<ArgTypes>()...)` is well formed when treated as
/// an unevaluated operand, and is known not to throw any exceptions.
template<typename F, typename ...Args>
struct is_nothrow_invocable : detail::is_invocable_r_impl<void, F, Args...>::nothrow_invocable {};



/// @brief Determines whether `INVOKE<R>(std::declval<Fn>(), std::declval<ArgTypes>()...)` is well formed when treated
/// as an unevaluated operand, and is known not to throw any exceptions.
template<typename R, typename F, typename ...Args>
struct is_nothrow_invocable_r : detail::is_invocable_r_impl<R, F, Args...>::nothrow_invocable {};

/// @}
/// @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_IS_INVOCABLE_HPP
