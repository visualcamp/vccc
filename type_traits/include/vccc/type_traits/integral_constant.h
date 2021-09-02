# /*
#  * Created by YongGyu Lee on 2021/09/03.
#  */
#
# ifndef VCCC_TYPE_TRAITS_INTEGRAL_CONSTANT_H_
# define VCCC_TYPE_TRAITS_INTEGRAL_CONSTANT_H_

namespace vccc {

template<typename T, T v>
struct integral_constant {
  using value_type = T;
  using type = integral_constant;

  static constexpr value_type value = v;

  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};

template<bool B>
using bool_constant = integral_constant<bool, B>;

using true_type  = integral_constant<bool, true >;
using false_type = integral_constant<bool, false>;

template<typename ...> struct always_false : false_type {};
template<typename ...> struct always_true : true_type {};

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_INTEGRAL_CONSTANT_H_
