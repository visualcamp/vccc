#ifndef VCCC_FUNCTIONAL_BIND_BACK_HPP_
#define VCCC_FUNCTIONAL_BIND_BACK_HPP_

#include <type_traits>
#include <utility>

#include "vccc/__functional/detail/bind_base.hpp"

namespace vccc {
namespace detail {

template<typename F, typename... Args>
class bind_back_object : public bind_object_base<bind_back_object<F, Args...>, F, Args...> {
  using base = bind_object_base<bind_back_object<F, Args...>, F, Args...>;
  friend base;

  template<typename Self, typename... U>
  struct bind_invoke_result
      : invoke_result<copy_cvref_t<Self&&, F>, U&&..., copy_cvref_t<Self&&, Args>...> {};

  template<typename Self, typename... U>
  struct bind_nothrow_invocable
      : is_nothrow_invocable<copy_cvref_t<Self&&, F>, U&&..., copy_cvref_t<Self&&, Args>...> {};

  template<std::size_t...I, typename Self, typename... U>
  static constexpr typename bind_invoke_result<Self&&, U&&...>::type
  call(Self&& self, std::index_sequence<I...>, U&&... args) noexcept(bind_nothrow_invocable<Self&&, U&&...>::value) {
    return vccc::invoke(
        std::forward<Self>(self).func_,
        std::forward<U>(args)...,
        std::get<I>(std::forward<Self>(self).args_)...
    );
  }

 public:
  using base::base;
  using base::operator();
};

} // namespace detail

/// @addtogroup functional
/// @{

template<typename F, typename... Args, std::enable_if_t<std::is_constructible<std::decay_t<F>, F>::value, int> = 0>
constexpr auto bind_back(F&& f, Args&&... args) {
  return detail::bind_back_object<std::decay_t<F>, std::decay_t<Args>...>{
      detail::bind_object_ctor_tag{},
      std::forward<F>(f),
      std::forward<Args>(args)...
  };
}

/// @}

} // namespace vccc

#endif // VCCC_FUNCTIONAL_BIND_BACK_HPP_
