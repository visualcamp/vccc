# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MAT_EXPRESSION_HPP
# define VCCC_MATH_MAT_EXPRESSION_HPP
#
# include <cstddef>
# include <type_traits>

namespace vccc{

template<typename Derived>
class MatExpression {
 public:
  using derived_traits = internal::math::traits<Derived>;

  enum {
    rows = derived_traits::rows,
    cols = derived_traits::cols,
    size = rows * cols,
    shortdim = rows < cols ? rows : cols
  };

  static_assert((rows > 0 && cols > 0), "matrix size must be greater than 0");

  //! static polymorphic virtual-like member functions
  constexpr inline decltype(auto) operator() (std::size_t i) const {
    return static_cast<const Derived&>(*this)(i);
  }
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j) const {
    return static_cast<const Derived&>(*this)(i, j);
  }
  constexpr inline decltype(auto) operator[] (std::size_t i) const {
    return static_cast<const Derived&>(*this)[i];
  }

};


namespace internal { namespace math {

template<typename Derived>
std::true_type is_matrix_impl(const vccc::MatExpression<Derived>&);
std::false_type is_matrix_impl(...);

}} // namespace internal::math

template<typename T>
struct is_matrix : decltype(::vccc::internal::math::is_matrix_impl(std::declval<T>())) {};

}


#endif //VCCC_MATH_MAT_EXPRESSION_HPP
