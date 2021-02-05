# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_ALGEBRA_MAT_EXPRESSION_HPP
# define VCCC_MATH_ALGEBRA_MAT_EXPRESSION_HPP
#
# include <cstddef>

namespace vccc{

template<typename T, int m, int n>
class MatExpression {
 public:
  enum {
    rows = m,
    cols = n,
    size = m * n
  };

  // operator() (std::size_t)
  constexpr inline decltype(auto) operator() (std::size_t i) const {
    return static_cast<const T&>(*this)(i);
  }
  constexpr inline decltype(auto) operator() (std::size_t i) {
    return static_cast<T&>(*this)(i);
  }

  // operator() (std::size_t, std::size_t)
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j) const {
    return static_cast<const T&>(*this)(i, j);
  }
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j) {
    return static_cast<T&>(*this)(i, j);
  }

  // operator[] (std::size_t)
  constexpr inline decltype(auto) operator[] (std::size_t i) const {
    return static_cast<const T&>(*this)[i];
  }
  constexpr inline decltype(auto) operator[] (std::size_t i) {
    return static_cast<T&>(*this)[i];
  }

};

}


#endif //VCCC_MATH_ALGEBRA_MAT_EXPRESSION_HPP
