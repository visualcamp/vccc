# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_HPP
# define VCCC_MATH_MATRIX_MATRIX_HPP
#
# include "vccc/math/matrix/matrix_base.hpp"
# include "vccc/math/matrix/matrix_assigner.hpp"
# include "vccc/math/matrix/assert.hpp"

namespace vccc {

namespace internal { namespace math {

template<typename T, int m, int n>
struct traits<Matrix<T, m, n>> {
  enum {
    rows = m,
    cols = n,
    size = rows * cols,
  };

  enum {
    option = Flag::kDefault
  };
  using value_type = T;
};

}} // namespace internal::math

struct matrix_ctor_all_t {};
struct matrix_ctor_diag_t {};
struct matrix_ctor_matmul_t {};

constexpr matrix_ctor_all_t matrix_ctor_all;
constexpr matrix_ctor_diag_t matrix_ctor_diag;
constexpr matrix_ctor_matmul_t matrix_ctor_matmul;

template<typename T, int m, int n>
class Matrix : public MatrixBase<Matrix<T, m, n>> {
 public:
  using value_type = T;
  using matrix_type = Matrix<T, m, n>;
  using base_type = MatrixBase<matrix_type>;
  using diag_type = Matrix<T, Matrix::shortdim, 1>;
  using base_type::rows;
  using base_type::cols;
  using base_type::size;
  using base_type::shortdim;

  // default ctor with all value 0
  constexpr Matrix();

  constexpr explicit Matrix(T v0);
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

  template<std::size_t N>
  constexpr explicit Matrix(const T(& arr)[N]);

//  template<typename U, std::enable_if_t<!is_matrix<std::decay_t<U>>::value, int> = 0> constexpr explicit Matrix(U&& v0);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3, U&& v4);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3, U&& v4, U&& v5);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3, U&& v4, U&& v5, U&& v6);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3, U&& v4, U&& v5, U&& v6, U&& v7);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3, U&& v4, U&& v5, U&& v6, U&& v7, U&& v8);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3, U&& v4, U&& v5, U&& v6, U&& v7, U&& v8, U&& v9);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3, U&& v4, U&& v5, U&& v6, U&& v7, U&& v8, U&& v9, U&& v10);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3, U&& v4, U&& v5, U&& v6, U&& v7, U&& v8, U&& v9, U&& v10, U&& v11);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3, U&& v4, U&& v5, U&& v6, U&& v7, U&& v8, U&& v9, U&& v10, U&& v11, U&& v12);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3, U&& v4, U&& v5, U&& v6, U&& v7, U&& v8, U&& v9, U&& v10, U&& v11, U&& v12, U&& v13);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3, U&& v4, U&& v5, U&& v6, U&& v7, U&& v8, U&& v9, U&& v10, U&& v11, U&& v12, U&& v13, U&& v14);
//  template<typename U> constexpr Matrix(U&& v0, U&& v1, U&& v2, U&& v3, U&& v4, U&& v5, U&& v6, U&& v7, U&& v8, U&& v9, U&& v10, U&& v11, U&& v12, U&& v13, U&& v14, U&& v15);

  constexpr Matrix(matrix_ctor_all_t, T value);
  constexpr Matrix(matrix_ctor_diag_t, const diag_type& value);
  constexpr Matrix(matrix_ctor_all_t, matrix_ctor_diag_t, T value);

//  template<typename LhsType, typename RhsType, int l>
//  constexpr Matrix(matrix_ctor_matmul_t, const MatrixMulMatrix<LhsType, RhsType, m, l, n>& );

  template<typename E>
  constexpr Matrix(const MatrixBase<E>& expr);

  template<typename E>
  constexpr Matrix& operator = (const MatrixBase<E>& expr);

  inline MatrixProxyNocopy<matrix_type> noAlias() {
    return MatrixProxyNocopy<matrix_type>(*this);
  }

  constexpr static Matrix all(T value);
  constexpr static Matrix zeros();
  constexpr static Matrix ones();
  constexpr static Matrix eye();
  constexpr static Matrix diag(const diag_type& value);


  constexpr inline decltype(auto) operator[](std::size_t i) const {
    VCCC_MATH_ASSERT_1D_MATRIX(matrix_type);
    return data[i];
  }
  constexpr inline decltype(auto) operator[](std::size_t i) {
    VCCC_MATH_ASSERT_1D_MATRIX(matrix_type);
    return data[i];
  }

  constexpr inline decltype(auto) operator()(std::size_t i) const { return data[i]; }
  constexpr inline decltype(auto) operator()(std::size_t i) { return data[i]; }

  constexpr inline decltype(auto) operator()(std::size_t i, std::size_t j) const { return data[i * matrix_type::cols + j]; }
  constexpr inline decltype(auto) operator()(std::size_t i, std::size_t j) { return data[i * matrix_type::cols + j]; }

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

//template<typename T, int m, int n>
//template<typename U, std::enable_if_t<!is_matrix<std::decay_t<U>>::value, int>>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0)
//    : data{std::forward<U>(v0)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1)
//    : data{std::forward<U>(v0), std::forward<U>(v1)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3,
//    U&& v4)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3),
//           std::forward<U>(v4)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3,
//    U&& v4, U&& v5)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3),
//           std::forward<U>(v4), std::forward<U>(v5)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3,
//    U&& v4, U&& v5, U&& v6)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3),
//           std::forward<U>(v4), std::forward<U>(v5), std::forward<U>(v6)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3,
//    U&& v4, U&& v5, U&& v6, U&& v7)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3),
//           std::forward<U>(v4), std::forward<U>(v5), std::forward<U>(v6), std::forward<U>(v7)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3,
//    U&& v4, U&& v5, U&& v6, U&& v7,
//    U&& v8)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3),
//           std::forward<U>(v4), std::forward<U>(v5), std::forward<U>(v6), std::forward<U>(v7),
//           std::forward<U>(v8)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3,
//    U&& v4, U&& v5, U&& v6, U&& v7,
//    U&& v8, U&& v9)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3),
//           std::forward<U>(v4), std::forward<U>(v5), std::forward<U>(v6), std::forward<U>(v7),
//           std::forward<U>(v8), std::forward<U>(v9)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3,
//    U&& v4, U&& v5, U&& v6, U&& v7,
//    U&& v8, U&& v9, U&& v10)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3),
//           std::forward<U>(v4), std::forward<U>(v5), std::forward<U>(v6), std::forward<U>(v7),
//           std::forward<U>(v8), std::forward<U>(v9), std::forward<U>(v10)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3,
//    U&& v4, U&& v5, U&& v6, U&& v7,
//    U&& v8, U&& v9, U&& v10, U&& v11)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3),
//           std::forward<U>(v4), std::forward<U>(v5), std::forward<U>(v6), std::forward<U>(v7),
//           std::forward<U>(v8), std::forward<U>(v9), std::forward<U>(v10), std::forward<U>(v11)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3,
//    U&& v4, U&& v5, U&& v6, U&& v7,
//    U&& v8, U&& v9, U&& v10, U&& v11,
//    U&& v12)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3),
//           std::forward<U>(v4), std::forward<U>(v5), std::forward<U>(v6), std::forward<U>(v7),
//           std::forward<U>(v8), std::forward<U>(v9), std::forward<U>(v10), std::forward<U>(v11),
//           std::forward<U>(v12)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3,
//    U&& v4, U&& v5, U&& v6, U&& v7,
//    U&& v8, U&& v9, U&& v10, U&& v11,
//    U&& v12, U&& v13)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3),
//           std::forward<U>(v4), std::forward<U>(v5), std::forward<U>(v6), std::forward<U>(v7),
//           std::forward<U>(v8), std::forward<U>(v9), std::forward<U>(v10), std::forward<U>(v11),
//           std::forward<U>(v12), std::forward<U>(v13)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3,
//    U&& v4, U&& v5, U&& v6, U&& v7,
//    U&& v8, U&& v9, U&& v10, U&& v11,
//    U&& v12, U&& v13, U&& v14)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3),
//           std::forward<U>(v4), std::forward<U>(v5), std::forward<U>(v6), std::forward<U>(v7),
//           std::forward<U>(v8), std::forward<U>(v9), std::forward<U>(v10), std::forward<U>(v11),
//           std::forward<U>(v12), std::forward<U>(v13), std::forward<U>(v14)} {}
//
//template<typename T, int m, int n>
//template<typename U>
//constexpr Matrix<T, m, n>::Matrix(
//    U&& v0, U&& v1, U&& v2, U&& v3,
//    U&& v4, U&& v5, U&& v6, U&& v7,
//    U&& v8, U&& v9, U&& v10, U&& v11,
//    U&& v12, U&& v13, U&& v14, U&& v15)
//    : data{std::forward<U>(v0), std::forward<U>(v1), std::forward<U>(v2), std::forward<U>(v3),
//           std::forward<U>(v4), std::forward<U>(v5), std::forward<U>(v6), std::forward<U>(v7),
//           std::forward<U>(v8), std::forward<U>(v9), std::forward<U>(v10), std::forward<U>(v11),
//           std::forward<U>(v12), std::forward<U>(v13), std::forward<U>(v14), std::forward<U>(v15)} {}


template<typename T, int m, int n>
template<std::size_t N>
constexpr
Matrix<T, m, n>::Matrix(const T (& arr)[N]) : data{} {
  static_assert(N <= m * n, "array size must not bigger than size of the matrix");

  int i = 0;
  for (; i < N; ++i)
    data[i] = arr[i];
}

template<typename T, int m, int n>
template<typename E>
constexpr
Matrix<T, m, n>::Matrix(const MatrixBase<E>& expr) {
  VCCC_MATH_ASSERT_SAME_SIZE(E, matrix_type);
  MatrixAssigner::assign(expr, *this);
}

template<typename T, int m, int n>
template<typename E>
constexpr
Matrix<T, m, n>&
Matrix<T, m, n>::operator=(const MatrixBase<E>& expr) {
  VCCC_MATH_ASSERT_SAME_SIZE(E, matrix_type);
  MatrixAssigner::assign(expr, *this);
  return *this;
}


template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(matrix_ctor_all_t, value_type value)
  : data{}
{
  for (int i = 0; i < size; ++i)
    data[i] = value;
}

template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(matrix_ctor_all_t, matrix_ctor_diag_t, value_type value)
  : data{}
{
  for (int i = 0; i < shortdim; ++i)
    data[i * rows + i] = value;
}

template<typename T, int m, int n>
constexpr Matrix<T, m, n>::Matrix(matrix_ctor_diag_t, const Matrix::diag_type& value)
  : data{}
{
  for (int i = 0; i < shortdim; ++i)
    data[i * rows + i] = value[i];
}

//! static Matrix make functions
template<typename T, int m, int n>
constexpr
Matrix<T, m, n>
Matrix<T, m, n>::all(T value) {
  return Matrix<T, m, n>(matrix_ctor_all, value);
}

template<typename T, int m, int n>
constexpr Matrix<T, m, n>
Matrix<T, m, n>::zeros() {
  return all(T(0));
}

template<typename T, int m, int n>
constexpr Matrix<T, m, n>
Matrix<T, m, n>::ones() {
  return all(T(1));
}

template<typename T, int m, int n>
constexpr Matrix<T, m, n>
Matrix<T, m, n>::eye() {
  static_assert(m == n, "Eye matrix must be square");
  return Matrix<T, m, n>(matrix_ctor_all, matrix_ctor_diag, 1);
}

template<typename T, int m, int n>
constexpr Matrix<T, m, n>
Matrix<T, m, n>::diag(const Matrix::diag_type& value) {
  return Matrix<T, m, n>(matrix_ctor_diag, value);
}

//! matrix out-of-class operations

template<typename T, int m, int n>
static inline
Matrix<T, m, n>&
operator *= (Matrix<T, m, n>& mat, T val) {
  // TODO: use MatrixAssigner or raw access?
//  MatrixAssigner::assign(mat*val, mat);
  for(int i=0; i<mat.size; ++i)
    mat.data[i] *= val;
  return mat;
}

template<typename T, int m, int n>
static inline
Matrix<T, m, n>&
operator /= (Matrix<T, m, n>& mat, T val) {
  for(int i=0; i<mat.size; ++i)
    mat.data[i] /= val;
  return mat;
}

template<typename E, typename T, int m, int n>
static inline
Matrix<T, m, n>&
operator += (Matrix<T, m, n>& mat, const MatrixBase<E>& expr) {
  for(int i=0; i<mat.size; ++i)
    mat.data[i] += expr[i];
  return mat;
}

template<typename E, typename T, int m, int n>
static inline
Matrix<T, m, n>&
operator -= (Matrix<T, m, n>& mat, const MatrixBase<E>& expr) {
  for(int i=0; i<mat.size; ++i)
    mat.data[i] -= expr[i];
  return mat;
}



}

# endif //VCCC_MATH_MATRIX_MATRIX_HPP
