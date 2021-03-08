# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_MINUS_HPP
# define VCCC_MATH_MATRIX_MATRIX_MINUS_HPP
#
# include "vccc/math/matrix/mat_expression.hpp"
# include "vccc/math/matrix/type_helper.hpp"

namespace vccc {

namespace internal{ namespace math {

template<typename E>
struct traits<MatrixMinus<E>> {
  enum {
    rows = traits<E>::rows,
    cols = traits<E>::cols
  };

  enum {
    option = traits<E>::option | Flag::kAliasUnsafe
  };
  using value_type = typename E::value_type;
};

}}


template<typename E>
class MatrixMinus : public MatExpression<MatrixMinus<E>> {
  const E& e;

 public:
  using lhs_type = internal::math::hold_type_selector_t<E>;


  constexpr inline explicit MatrixMinus(const E& e) : e(e) {}

  constexpr inline decltype(auto) operator() (std::size_t i) const { return -e(i); }
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j) const { return -e(i, j); }
  constexpr inline decltype(auto) operator[] (std::size_t i) const { return -e[i]; }
};

template<typename E>
constexpr static inline
MatrixMinus<E>
operator - (const MatExpression<E>& lhs) {
  return MatrixMinus<E>(*static_cast<const E*>(&lhs));
}

}



# endif //VCCC_MATH_MATRIX_MATRIX_MINUS_HPP
