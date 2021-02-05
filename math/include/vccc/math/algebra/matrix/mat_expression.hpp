# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_ALGEBRA_MAT_EXPRESSION_HPP
# define VCCC_MATH_ALGEBRA_MAT_EXPRESSION_HPP
#
# include <cstddef>
# include "vccc/math/algebra/matrix/forward_declare.hpp"

namespace vccc{

template<typename Crt, int m, int n>
class MatExpression {
 public:
  enum {
    rows = m,
    cols = n,
    size = m * n
  };

  static_assert((m > 0 && n > 0) || size > 1, "matrix size must be greater than 1");


  //! static polymorphic virtual-like member functions
  // operator() (std::size_t)
  constexpr inline decltype(auto) operator() (std::size_t i) const;
  constexpr inline decltype(auto) operator() (std::size_t i);

  // operator() (std::size_t, std::size_t)
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j) const;
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j);

  // operator[] (std::size_t)
  constexpr inline decltype(auto) operator[] (std::size_t i) const;
  constexpr inline decltype(auto) operator[] (std::size_t i);

  // unary minus operator
  constexpr inline MatrixSub<Crt, Crt, matrix_sub_unary, m, n> operator - () const;
};

template<typename Crt, int m, int n>
constexpr inline decltype(auto)
MatExpression<Crt, m, n>::operator() (std::size_t i) const {
  return static_cast<const Crt&>(*this)(i);
}

template<typename Crt, int m, int n>
constexpr inline decltype(auto)
MatExpression<Crt, m, n>::operator() (std::size_t i) {
  return static_cast<Crt&>(*this)(i);
}

template<typename Crt, int m, int n>
constexpr inline decltype(auto)
MatExpression<Crt, m, n>::operator() (std::size_t i, std::size_t j) const {
  return static_cast<const Crt&>(*this)(i, j);
}

template<typename Crt, int m, int n>
constexpr inline decltype(auto)
MatExpression<Crt, m, n>::operator() (std::size_t i, std::size_t j) {
  return static_cast<Crt&>(*this)(i, j);
}

template<typename Crt, int m, int n>
constexpr inline decltype(auto)
MatExpression<Crt, m, n>::operator[] (std::size_t i) const {
  return static_cast<const Crt&>(*this)[i];
}

template<typename Crt, int m, int n>
constexpr inline decltype(auto)
MatExpression<Crt, m, n>::operator[] (std::size_t i) {
  return static_cast<Crt&>(*this)[i];
}

// unary minus operator
template<typename Crt, int m, int n>
constexpr inline
MatrixSub<Crt, Crt, matrix_sub_unary, m, n>
MatExpression<Crt, m, n>::operator - () const {
  return MatrixSub<Crt, Crt, matrix_sub_unary, m, n>(*static_cast<const Crt*>(this), *static_cast<const Crt*>(this));
}




}


#endif //VCCC_MATH_ALGEBRA_MAT_EXPRESSION_HPP
