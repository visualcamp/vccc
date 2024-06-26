#ifndef VCCC_FUNCTIONAL_DETAIL_BIND_BASE_HPP_
#define VCCC_FUNCTIONAL_DETAIL_BIND_BASE_HPP_

#include <tuple>
#include <utility>

#include "vccc/__functional/invoke.hpp"
#include "vccc/__type_traits/copy_cvref.hpp"

namespace vccc {
namespace detail {

struct bind_object_ctor_tag{};

template<typename Derived, typename F, typename... Args>
class bind_object_base {
  template<typename DerivedRef, typename... U>
  struct bind_invoke_result : Derived::template bind_invoke_result<DerivedRef, U&&...> {};

  template<typename DerivedRef, typename... U>
  struct bind_nothrow_invocable : Derived::template bind_nothrow_invocable<DerivedRef, U&&...> {};

  using index_sequence = std::index_sequence_for<Args...>;

 public:
  template<typename F2, typename... Args2>
  constexpr bind_object_base(bind_object_ctor_tag, F2&& func, Args2&&... args)
      : func_(std::forward<F2>(func))
      , args_(std::forward<Args2>(args)...) {}

  template<typename... U>
  constexpr typename bind_invoke_result<Derived&, U&&...>::type
  operator()(U&&... args) & noexcept(bind_nothrow_invocable<Derived&, U&&...>::value) {
    return call(*this, std::forward<U>(args)...);
    return Derived::call(derived(), index_sequence{}, std::forward<U>(args)...);
  }

  template<typename... U>
  constexpr typename bind_invoke_result<const Derived&, U&&...>::type
  operator()(U&&... args) const & noexcept(bind_nothrow_invocable<const Derived&, U&&...>::value) {
    return call(*this, std::forward<U>(args)...);
  }

  template<typename... U>
  constexpr typename bind_invoke_result<Derived&&, U&&...>::type
  operator()(U&&... args) && noexcept(bind_nothrow_invocable<Derived&&, U&&...>::value) {
    return call(std::move(*this), std::forward<U>(args)...);
  }

  template<typename... U>
  constexpr typename bind_invoke_result<const Derived&&, U&&...>::type
  operator()(U&&... args) const && noexcept(bind_nothrow_invocable<const Derived&&, U&&...>::value) {
    return call(std::move(*this), std::forward<U>(args)...);
  }

 protected:
  F func_;
  std::tuple<Args...> args_;

 private:
  template<typename Self, typename... U>
  static constexpr typename bind_invoke_result<copy_cvref_t<Self&&, Derived>, U&&...>::type
  call(Self&& self, U&&... args) noexcept(bind_nothrow_invocable<copy_cvref_t<Self&&, Derived>, U&&...>::value) {
    return Derived::call(std::forward<Self>(self).derived(), index_sequence{}, std::forward<U>(args)...);
  }

  Derived& derived() & { return static_cast<Derived&>(*this); }
  const Derived& derived() const & { return static_cast<const Derived&>(*this); }
  Derived&& derived() && { return static_cast<Derived&&>(*this); }
  const Derived&& derived() const && { return static_cast<const Derived&&>(*this); }
};

} // namespace detail
} // namespace vccc

#endif // VCCC_FUNCTIONAL_DETAIL_BIND_BASE_HPP_
