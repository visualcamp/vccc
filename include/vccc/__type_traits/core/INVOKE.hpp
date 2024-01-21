//
// Created by yonggyulee on 2023/12/29.
//

#ifndef VCCC_TYPE_TRAITS_CORE_INVOKE_HPP
#define VCCC_TYPE_TRAITS_CORE_INVOKE_HPP

#include <functional>
#include <type_traits>
#include <utility>

#include "vccc/__type_traits/is_specialization.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

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
    get_invoke_tag_1<std::decay_t<F>>::value,
    get_invoke_tag_2<typename get_class_type_or<std::decay_t<F>>::type, std::decay_t<T1>>::value> {};


template<typename F, typename ...Args>
auto INVOKE(F&& f, Args&&... args)
    noexcept(noexcept(invoke_concrete<F, Args...>::INVOKE(std::declval<F>(),  std::declval<Args>()...)     ))
          -> decltype(invoke_concrete<F, Args...>::INVOKE(std::declval<F>(),  std::declval<Args>()...)      )
             { return invoke_concrete<F, Args...>::INVOKE(std::forward<F>(f), std::forward<Args>(args)...); }

} // namespace detail
} // namespace vccc

#endif // VCCC_TYPE_TRAITS_CORE_INVOKE_HPP
