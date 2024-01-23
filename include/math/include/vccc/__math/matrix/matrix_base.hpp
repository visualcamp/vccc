# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MAT_EXPRESSION_HPP
# define VCCC_MATH_MAT_EXPRESSION_HPP
#
# include <cstddef>
# include <tuple>
# include <type_traits>
#
# include "vccc/__math/matrix/forward_declare.hpp"

namespace vccc {

//! @addtogroup math_matrix
//! @{

template<typename Derived>
class MatrixBase {
 public:
  using derived_type = Derived;
  using derived_traits = internal::math::traits<derived_type>;
  using value_type = typename derived_traits::value_type;

  enum {
    rows = derived_traits::rows,
    cols = derived_traits::cols,
    size = rows * cols,
    shortdim = rows < cols ? rows : cols
  };

  static_assert((rows > 0 && cols > 0), "matrix size must be greater than 0");

  //! static polymorphic virtual-like member functions
  constexpr value_type operator() (std::size_t i) const {
    return derived()(i);
  }
  constexpr value_type operator() (std::size_t i, std::size_t j) const {
    return derived()(i, j);
  }
  constexpr value_type operator[] (std::size_t i) const {
    return derived()[i];
  }

  constexpr inline const derived_type& derived() const {
    return static_cast<const derived_type&>(*this);
  }
  constexpr inline derived_type& derived() {
    return static_cast<derived_type&>(*this);
  }
};

//! @} math_matrix

namespace internal {
namespace math {

template<typename Derived>
std::true_type is_matrix_impl(const vccc::MatrixBase<Derived>&);
std::false_type is_matrix_impl(...);

} // namespace math
} // namespace internal

template<typename T>
struct is_matrix : decltype(::vccc::internal::math::is_matrix_impl(std::declval<T>())) {};

} // namespace vccc

template<typename Derived>
struct std::tuple_size<vccc::MatrixBase<Derived>>
  : std::integral_constant<std::size_t, vccc::MatrixBase<Derived>::size> {};

namespace std {

template<std::size_t I, typename Derived>
constexpr inline std::enable_if_t<(I < vccc::MatrixBase<Derived>::size), const typename vccc::MatrixBase<Derived>::value_type&>
get(const vccc::MatrixBase<Derived>& m) {
  return m[I];
}

} // namespace std

#endif // VCCC_MATH_MAT_EXPRESSION_HPP
