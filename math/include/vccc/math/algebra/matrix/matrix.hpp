# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_ALGEBRA_MATRIX_MATRIX_HPP
# define VCCC_MATH_ALGEBRA_MATRIX_MATRIX_HPP
#
# include "vccc/math/algebra/matrix/mat_expression.hpp"

namespace vccc {

struct MatrixCtor_All {};

template<typename T, int m, int n>
class Matrix : public MatExpression<Matrix<T, m, n>, m, n> {
 public:
  using value_type = T;

  // default ctor with all value 0
  constexpr Matrix();

  constexpr Matrix(MatrixCtor_All, value_type value);

  template<std::size_t N>
  constexpr Matrix(const T(&arr)[N]);

  constexpr static Matrix all(T value);

  template<typename E>
  constexpr Matrix(const MatExpression<E, m, n>& expr);

  constexpr inline const T* begin()   const { return data; }
  constexpr inline       T* begin()         { return data; }
  constexpr inline const T* cbegin()  const { return data; }

  constexpr inline const T* end()     const { return data + this->size; }
  constexpr inline       T* end()           { return data + this->size; }
  constexpr inline const T* cend()    const { return data + this->size; }


  constexpr inline decltype(auto) operator[] (std::size_t i) const { return data[i]; }
  constexpr inline decltype(auto) operator[] (std::size_t i)       { return data[i]; }

  constexpr inline decltype(auto) operator() (std::size_t i) const { return data[i]; }
  constexpr inline decltype(auto) operator() (std::size_t i)       { return data[i]; }

  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j) const { return data[i * this->cols + j]; }
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j)       { return data[i * this->cols + j]; }

// private:
  T data[m*n];
};


template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix() : data{} {}

template<typename T, int m, int n>
template<std::size_t N>
constexpr
Matrix<T, m, n>::Matrix(const T (& arr)[N]) {
  static_assert(N <= m*n, "array size must not bigger than size of the matrix");

  int i=0;
  for(; i<N; ++i)
    data[i] = arr[i];

  for(; i<this->size; ++i)
    data[i] = T(0);
}

template<typename T, int m, int n>
template<typename E>
constexpr
Matrix<T, m, n>::Matrix(const MatExpression<E, m, n>& expr) {
  for(int i=0; i<expr.size(); ++i)
    data[i] = expr[i];
}

template<typename T, int m, int n>
constexpr
Matrix<T, m, n>
Matrix<T, m, n>::all(T value) {
  return Matrix<T, m, n>(MatrixCtor_All{}, value);
}

template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(MatrixCtor_All, value_type value) {
  for(int i=0; i<this->size; ++i)
    data[i] = value;
}

}

# endif //VCCC_MATH_ALGEBRA_MATRIX_MATRIX_HPP
