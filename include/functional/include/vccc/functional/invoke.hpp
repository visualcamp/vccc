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
# include "vccc/type_traits/conjunction.hpp"
# include "vccc/type_traits/disjunction.hpp"
# include "vccc/type_traits/empty.hpp"
# include "vccc/type_traits/is_complete.hpp"
# include "vccc/type_traits/is_specialization.hpp"
# include "vccc/type_traits/is_unbounded_array.hpp"
# include "vccc/type_traits/negation.hpp"
# include "vccc/type_traits/remove_cvref.hpp"
# include "vccc/type_traits/type_identity.hpp"

namespace vccc {

namespace detail {

enum class invoke_tag_1 {
  pointer_to_function,
  pointer_to_data_member,
  function_object,
};

enum class invoke_tag_2 {
  object,
  reference_wrapper,
  ptr_or_else,
};

template<invoke_tag_1, invoke_tag_2>
struct invoke_impl;

template<>
struct invoke_impl<invoke_tag_1::pointer_to_function, invoke_tag_2::object> {
  template<typename F, typename T1, typename ...Ts>
  static constexpr auto INVOKE(F f, T1&& t1, Ts&&... ts)
      noexcept(noexcept((std::forward<T1>(t1).*f)(std::forward<Ts>(ts)...)))
            -> decltype((std::forward<T1>(t1).*f)(std::forward<Ts>(ts)...))
               { return (std::forward<T1>(t1).*f)(std::forward<Ts>(ts)...); }
};

template<>
struct invoke_impl<invoke_tag_1::pointer_to_function, invoke_tag_2::reference_wrapper> {
  template<typename F, typename T1, typename ...Ts>
  static constexpr auto INVOKE(F f, T1&& t1, Ts&&... ts)
      noexcept(noexcept((std::forward<T1>(t1).get().*f)(std::forward<Ts>(ts)...)))
            -> decltype((std::forward<T1>(t1).get().*f)(std::forward<Ts>(ts)...))
               { return (std::forward<T1>(t1).get().*f)(std::forward<Ts>(ts)...); }
};

template<>
struct invoke_impl<invoke_tag_1::pointer_to_function, invoke_tag_2::ptr_or_else> {
  template<typename F, typename T1, typename ...Ts>
  static constexpr auto INVOKE(F f, T1&& t1, Ts&&... ts)
      noexcept(noexcept(((*std::forward<T1>(t1)).*f)(std::forward<Ts>(ts)...)))
            -> decltype(((*std::forward<T1>(t1)).*f)(std::forward<Ts>(ts)...))
               { return ((*std::forward<T1>(t1)).*f)(std::forward<Ts>(ts)...); }
};

template<>
struct invoke_impl<invoke_tag_1::pointer_to_data_member, invoke_tag_2::object> {
  template<typename F, typename T1>
  static constexpr auto INVOKE(F f, T1&& t1)
      noexcept(noexcept((std::forward<T1>(t1).*f)))
            -> decltype((std::forward<T1>(t1).*f))
               { return (std::forward<T1>(t1).*f); }
};

template<>
struct invoke_impl<invoke_tag_1::pointer_to_data_member, invoke_tag_2::reference_wrapper> {
  template<typename F, typename T1>
  static constexpr auto INVOKE(F f, T1&& t1)
      noexcept(noexcept((std::forward<T1>(t1).get().*f)))
            -> decltype((std::forward<T1>(t1).get().*f))
               { return (std::forward<T1>(t1).get().*f); }
};

template<>
struct invoke_impl<invoke_tag_1::pointer_to_data_member, invoke_tag_2::ptr_or_else> {
  template<typename F, typename T1>
  static constexpr auto INVOKE(F f, T1&& t1)
      noexcept(noexcept((*std::forward<T1>(t1).*f)))
            -> decltype((*std::forward<T1>(t1).*f))
               { return (*std::forward<T1>(t1).*f); }
};

template<invoke_tag_2 any>
struct invoke_impl<invoke_tag_1::function_object, any> {
  template<typename F, typename ...Ts>
  static constexpr auto INVOKE(F&& f, Ts&&... ts)
      noexcept(noexcept(std::forward<F>(f)(std::forward<Ts>(ts)...)))
            -> decltype(std::forward<F>(f)(std::forward<Ts>(ts)...))
               { return std::forward<F>(f)(std::forward<Ts>(ts)...); }
};

template<typename T> struct get_class_type_or { using type = T; };
template<typename R, typename C>
struct get_class_type_or<R (C::*)> { using type = C; };

template<typename F,
         bool v1 = std::is_member_function_pointer<F>::value,
         bool v2 = std::is_member_object_pointer<F>::value>
struct get_invoke_tag_1;

template<typename F>
struct get_invoke_tag_1<F, true, false> : std::integral_constant<invoke_tag_1, invoke_tag_1::pointer_to_function> {};

template<typename F>
struct get_invoke_tag_1<F, false, true> : std::integral_constant<invoke_tag_1, invoke_tag_1::pointer_to_data_member> {};

template<typename F>
struct get_invoke_tag_1<F, false, false> : std::integral_constant<invoke_tag_1, invoke_tag_1::function_object> {};

template<typename T, typename T1,
         bool v1 = std::is_base_of<T, std::remove_reference_t<T1>>::value,
         bool v2 = is_specialization<std::remove_cv_t<T1>, std::reference_wrapper>::value>
struct get_invoke_tag_2;

template<typename T, typename T1, bool any>
struct get_invoke_tag_2<T, T1, true, any> : std::integral_constant<invoke_tag_2, invoke_tag_2::object> {};

template<typename T, typename T1>
struct get_invoke_tag_2<T, T1, false, true> : std::integral_constant<invoke_tag_2, invoke_tag_2::reference_wrapper> {};

template<typename T, typename T1>
struct get_invoke_tag_2<T, T1, false, false> : std::integral_constant<invoke_tag_2, invoke_tag_2::ptr_or_else> {};

template<typename F, typename ...Ts>
struct invoke_concrete : invoke_impl<invoke_tag_1::function_object, invoke_tag_2::ptr_or_else> {};

template<typename F, typename T1, typename ...Ts>
struct invoke_concrete<F, T1, Ts...> : invoke_impl<
    get_invoke_tag_1<F>::value,
    get_invoke_tag_2<typename get_class_type_or<remove_cvref_t<F>>::type, T1>::value> {};


template<typename F, typename ...Args>
auto INVOKE(F&& f, Args&&... args)
    noexcept(noexcept(invoke_concrete<F, Args...>::INVOKE(std::declval<F>(),  std::declval<Args>()...)     ))
          -> decltype(invoke_concrete<F, Args...>::INVOKE(std::declval<F>(),  std::declval<Args>()...)      )
             { return invoke_concrete<F, Args...>::INVOKE(std::forward<F>(f), std::forward<Args>(args)...); }

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
  noexcept(noexcept(INVOKE(std::declval<F2>(), std::declval<Args2>()...)))
        -> decltype(INVOKE(std::declval<F2>(), std::declval<Args2>()...));
  template<typename F2, typename ...Args2>
  static auto test(...) -> empty;

 public:
  using test_return_type = decltype(test<F, Args...>(0));
  using convertible = disjunction<std::is_void<R>, std::is_convertible<test_return_type, R>>;
  using invocable = conjunction<negation<std::is_same<test_return_type, empty>>, convertible>;
  using nothrow_invocable = conjunction<invocable, bool_constant<noexcept(test<F, Args...>(0))>>;
};

} // namespace detail

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


/**
@addtogroup functional_invocable__class is_invocable, is_invocable_r, is_nothrow_invocable, is_nothrow_invocable_r
@brief checks if a type can be invoked (as if by `vccc::invoke`) with the given argument types
@{
 */

/// @brief Determines whether `INVOKE(std::declval<Fn>(), std::declval<ArgTypes>()...)` is well formed when treated as anunevaluated operand.
template<typename F, typename ...Args>
struct is_invocable : detail::is_invocable_r_impl<void, F, Args...>::invocable {};



/// @brief Determines whether `INVOKE<R>(std::declval<Fn>(), std::declval<ArgTypes>()...)` is well formed when treated as an unevaluated operand.
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
