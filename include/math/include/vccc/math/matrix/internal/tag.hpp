# /*
#  * Created by YongGyu Lee on 2020/03/02.
#  */
#
# ifndef VCCC_MATH_MATRIX_INTERNAL_TAG_HPP
# define VCCC_MATH_MATRIX_INTERNAL_TAG_HPP

namespace vccc {
namespace internal {
namespace math {

template<typename T, T ...val> struct tag {};

template<typename T, T v1> struct tag<T, v1> {
  using value_type = T;
  static constexpr value_type first = v1;
};

template<typename T, T v1, T v2> struct tag<T, v1, v2> : public tag<T, v1> {
  static constexpr T second = v2;
};

template<typename T, T v1, T v2, T v3> struct tag<T, v1, v2, v3> : public tag<T, v1, v2> {
  static constexpr T third = v3;
};

template<typename T, T v1, T v2, T v3, T v4> struct tag<T, v1, v2, v3, v4> : public tag<T, v1, v2, v3> {
  static constexpr T fourth = v4;
};

template<typename T, T v1, T v2, T v3, T v4, T v5> struct tag<T, v1, v2, v3, v4, v5> : public tag<T, v1, v2, v3, v4> {
  static constexpr T fifth = v5;
};

} // namespace math
} // namespace internal
} // namespace vccc

# endif // VCCC_MATH_MATRIX_INTERNAL_TAG_HPP
