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

  constexpr Matrix(T v0);
  constexpr Matrix(T v0, T v1);
  constexpr Matrix(T v0, T v1, T v2);
  constexpr Matrix(T v0, T v1, T v2, T v3);
  constexpr Matrix(T v0, T v1, T v2, T v3, T v4);
  constexpr Matrix(T v0, T v1, T v2, T v3, T v4, T v5);
  constexpr Matrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6);
  constexpr Matrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7);
  constexpr Matrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8);
  constexpr Matrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9);
  constexpr Matrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10);
  constexpr Matrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11);
  constexpr Matrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12);
  constexpr Matrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13);
  constexpr Matrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14);
  constexpr Matrix(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9, T v10, T v11, T v12, T v13, T v14, T v15);

  constexpr Matrix(MatrixCtor_All, value_type value);

  template<std::size_t N>
  constexpr Matrix(const T(& arr)[N]);

  constexpr static Matrix all(T value);
  constexpr static Matrix zeros();
  constexpr static Matrix ones();
  constexpr static Matrix eye();

  template<typename E>
  constexpr Matrix(const MatExpression<E, m, n>& expr);

//  constexpr inline const T* begin()   const { return data; }
//  constexpr inline       T* begin()         { return data; }
//  constexpr inline const T* cbegin()  const { return data; }
//
//  constexpr inline const T& rbegin() const  { return data + this->size - 1; }
//  constexpr inline       T& rbegin()        { return data + this->size - 1; }
//  constexpr inline const T& crbegin() const { return data + this->size - 1; }
//
//  constexpr inline const T* end()     const { return data + this->size; }
//  constexpr inline       T* end()           { return data + this->size; }
//  constexpr inline const T* cend()    const { return data + this->size; }
//
//  constexpr inline const T* rend()    const { return data - 1; }
//  constexpr inline       T* rend()          { return data - 1; }
//  constexpr inline const T* crend()   const { return data - 1; }


  constexpr inline decltype(auto) operator[](std::size_t i) const { return data[i]; }
  constexpr inline decltype(auto) operator[](std::size_t i) { return data[i]; }

  constexpr inline decltype(auto) operator()(std::size_t i) const { return data[i]; }
  constexpr inline decltype(auto) operator()(std::size_t i) { return data[i]; }

  constexpr inline decltype(auto) operator()(std::size_t i, std::size_t j) const { return data[i * this->cols + j]; }
  constexpr inline decltype(auto) operator()(std::size_t i, std::size_t j) { return data[i * this->cols + j]; }

// private:
  T data[m * n];
};

template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix() : data{} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0)
    : data{std::move(v0)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0, T v1)
    : data{std::move(v0), std::move(v1)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0, T v1, T v2)
    : data{std::move(v0), std::move(v1), std::move(v2)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0, T v1, T v2, T v3)
    : data{std::move(v0), std::move(v1), std::move(v2), std::move(v3)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0, T v1, T v2, T v3,
    T v4)
    : data{std::move(v0), std::move(v1), std::move(v2), std::move(v3),
           std::move(v4)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0, T v1, T v2, T v3,
    T v4, T v5)
    : data{std::move(v0), std::move(v1), std::move(v2), std::move(v3),
           std::move(v4), std::move(v5)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0, T v1, T v2, T v3,
    T v4, T v5, T v6)
    : data{std::move(v0), std::move(v1), std::move(v2), std::move(v3),
           std::move(v4), std::move(v5), std::move(v6)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0, T v1, T v2, T v3,
    T v4, T v5, T v6, T v7)
    : data{std::move(v0), std::move(v1), std::move(v2), std::move(v3),
           std::move(v4), std::move(v5), std::move(v6), std::move(v7)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0, T v1, T v2, T v3,
    T v4, T v5, T v6, T v7,
    T v8)
    : data{std::move(v0), std::move(v1), std::move(v2), std::move(v3),
           std::move(v4), std::move(v5), std::move(v6), std::move(v7),
           std::move(v8)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0, T v1, T v2, T v3,
    T v4, T v5, T v6, T v7,
    T v8, T v9)
    : data{std::move(v0), std::move(v1), std::move(v2), std::move(v3),
           std::move(v4), std::move(v5), std::move(v6), std::move(v7),
           std::move(v8), std::move(v9)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0, T v1, T v2, T v3,
    T v4, T v5, T v6, T v7,
    T v8, T v9, T v10)
    : data{std::move(v0), std::move(v1), std::move(v2), std::move(v3),
           std::move(v4), std::move(v5), std::move(v6), std::move(v7),
           std::move(v8), std::move(v9), std::move(v10)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0, T v1, T v2,  T v3,
    T v4, T v5, T v6,  T v7,
    T v8, T v9, T v10, T v11)
    : data{std::move(v0), std::move(v1), std::move(v2),  std::move(v3),
           std::move(v4), std::move(v5), std::move(v6),  std::move(v7),
           std::move(v8), std::move(v9), std::move(v10), std::move(v11)} {}

template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0, T v1, T v2,  T v3,
    T v4, T v5, T v6,  T v7,
    T v8, T v9, T v10, T v11,
    T v12)
    : data{std::move(v0), std::move(v1), std::move(v2),  std::move(v3),
           std::move(v4), std::move(v5), std::move(v6),  std::move(v7),
           std::move(v8), std::move(v9), std::move(v10), std::move(v11),
           std::move(v12)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0,  T v1, T v2,  T v3,
    T v4,  T v5, T v6,  T v7,
    T v8,  T v9, T v10, T v11,
    T v12, T v13)
    : data{std::move(v0),  std::move(v1), std::move(v2),  std::move(v3),
           std::move(v4),  std::move(v5), std::move(v6),  std::move(v7),
           std::move(v8),  std::move(v9), std::move(v10), std::move(v11),
           std::move(v12), std::move(v13)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0,  T v1,  T v2,  T v3,
    T v4,  T v5,  T v6,  T v7,
    T v8,  T v9,  T v10, T v11,
    T v12, T v13, T v14)
    : data{std::move(v0),  std::move(v1),  std::move(v2),  std::move(v3),
           std::move(v4),  std::move(v5),  std::move(v6),  std::move(v7),
           std::move(v8),  std::move(v9),  std::move(v10), std::move(v11),
           std::move(v12), std::move(v13), std::move(v14)} {}
template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(
    T v0,  T v1,  T v2,  T v3,
    T v4,  T v5,  T v6,  T v7,
    T v8,  T v9,  T v10, T v11,
    T v12, T v13, T v14, T v15)
    : data{std::move(v0),  std::move(v1),  std::move(v2),  std::move(v3),
           std::move(v4),  std::move(v5),  std::move(v6),  std::move(v7),
           std::move(v8),  std::move(v9),  std::move(v10), std::move(v11),
           std::move(v12), std::move(v13), std::move(v14), std::move(v15)} {}


template<typename T, int m, int n>
template<std::size_t N>
constexpr
Matrix<T, m, n>::Matrix(const T (& arr)[N]) {
  static_assert(N <= m * n, "array size must not bigger than size of the matrix");

  int i = 0;
  for (; i < N; ++i)
    data[i] = arr[i];

  for (; i < this->size; ++i)
    data[i] = T(0);
}

template<typename T, int m, int n>
template<typename E>
constexpr
Matrix<T, m, n>::Matrix(const MatExpression<E, m, n>& expr) {
  for (int i = 0; i < expr.size; ++i)
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
  for (int i = 0; i < this->size; ++i)
    data[i] = value;
}

template<typename T, int m, int n>
constexpr Matrix<T, m, n>
Matrix<T, m, n>::zeros() {
  return all(T(0));
}

}

# endif //VCCC_MATH_ALGEBRA_MATRIX_MATRIX_HPP
