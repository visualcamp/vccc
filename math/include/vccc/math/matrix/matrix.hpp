# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_HPP
# define VCCC_MATH_MATRIX_MATRIX_HPP
#
# include <utility>
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
struct matrix_ctor_array_t {};

constexpr matrix_ctor_all_t matrix_ctor_all;
constexpr matrix_ctor_diag_t matrix_ctor_diag;
constexpr matrix_ctor_matmul_t matrix_ctor_matmul;
constexpr matrix_ctor_array_t matrix_ctor_array;

/// @addtogroup math_matrix
/// @{

/**
 * A class that represents 2D matrix
 *
 * All matrix operations are using expression template
 *
 * @tparam T
 * @tparam m
 * @tparam n
 */
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

  /**
   * Create a zero matrix
   */
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

  constexpr explicit Matrix(const T(& arr)[size])
    : Matrix(matrix_ctor_array, arr, std::make_index_sequence<size>{}) {}

  template<std::size_t N, std::size_t... I>
  constexpr Matrix(matrix_ctor_array_t, const T(& arr)[N], std::index_sequence<I...>)
      : data{((void)I, arr[I])...} {}

  constexpr Matrix(matrix_ctor_all_t, T value);
  constexpr Matrix(matrix_ctor_diag_t, const diag_type& value);
  constexpr Matrix(matrix_ctor_all_t, matrix_ctor_diag_t, T value);

  template<typename E, std::enable_if_t<internal::math::is_same_size<matrix_type, E>::value, int> = 0>
  constexpr Matrix(const MatrixBase<E>& expr) {
    MatrixAssigner::assign(expr, *this);
  }

  template<typename E, std::enable_if_t<internal::math::is_same_size<matrix_type, E>::value, int> = 0>
  constexpr Matrix& operator=(const MatrixBase<E>& expr) {
    MatrixAssigner::assign(expr, *this);
    return *this;
  }

//  inline MatrixProxyNocopy<matrix_type> noAlias() {
//    return MatrixProxyNocopy<matrix_type>(*this);
//  }

  /**
   * Create all-value matrix
   */
  constexpr static Matrix all(T value) {
    return Matrix<T, m, n>(matrix_ctor_all, value);
  }

  /**
   * Create zero matrix
   */
  constexpr static Matrix zeros() {
    return all(T(0));
  }

  /**
   * Create all-ones matrix
   */
  constexpr static Matrix ones() {
    return all(T(1));
  }

  /**
   * Create a diagonal matrix
   */
  constexpr static Matrix diag(const diag_type& value) {
    return Matrix<T, m, n>(matrix_ctor_diag, value);
  }

  /**
   * Create an identity matrix
   */
  template<typename Dummy = void, std::enable_if_t<
      std::is_void<Dummy>::value &&
      (matrix_type::rows == matrix_type::cols), int> = 0>
  constexpr static Matrix eye() {
    return Matrix<T, m, n>(matrix_ctor_all, matrix_ctor_diag, 1);
  }

  constexpr       value_type& operator[](std::size_t i)       noexcept { return data[i]; }
  constexpr const value_type& operator[](std::size_t i) const noexcept { return data[i]; }

  constexpr       value_type& operator()(std::size_t i)       noexcept { return data[i]; }
  constexpr const value_type& operator()(std::size_t i) const noexcept { return data[i]; }

  constexpr       value_type& operator()(std::size_t i, std::size_t j)       noexcept { return data[i * matrix_type::cols + j]; }
  constexpr const value_type& operator()(std::size_t i, std::size_t j) const noexcept { return data[i * matrix_type::cols + j]; }

  /**
   * Create a matrix with a different type
   *
   * @tparam U New type
   * @return Matrix<U, m, n>
   */
  template<typename U>
  Matrix<U, m, n> AsType() const {
    return Matrix<U, m, n>(*this);
  }

 private:
  T data[m * n];
};

using Matrix12i = Matrix<int, 1, 2>;
using Matrix13i = Matrix<int, 1, 3>;
using Matrix14i = Matrix<int, 1, 4>;
using Matrix21i = Matrix<int, 2, 1>;
using Matrix22i = Matrix<int, 2, 2>;
using Matrix23i = Matrix<int, 2, 3>;
using Matrix24i = Matrix<int, 2, 4>;
using Matrix31i = Matrix<int, 3, 1>;
using Matrix32i = Matrix<int, 3, 2>;
using Matrix33i = Matrix<int, 3, 3>;
using Matrix34i = Matrix<int, 3, 4>;
using Matrix41i = Matrix<int, 4, 1>;
using Matrix42i = Matrix<int, 4, 2>;
using Matrix43i = Matrix<int, 4, 3>;
using Matrix44i = Matrix<int, 4, 4>;

using Matrix12f = Matrix<float, 1, 2>;
using Matrix13f = Matrix<float, 1, 3>;
using Matrix14f = Matrix<float, 1, 4>;
using Matrix21f = Matrix<float, 2, 1>;
using Matrix22f = Matrix<float, 2, 2>;
using Matrix23f = Matrix<float, 2, 3>;
using Matrix24f = Matrix<float, 2, 4>;
using Matrix31f = Matrix<float, 3, 1>;
using Matrix32f = Matrix<float, 3, 2>;
using Matrix33f = Matrix<float, 3, 3>;
using Matrix34f = Matrix<float, 3, 4>;
using Matrix41f = Matrix<float, 4, 1>;
using Matrix42f = Matrix<float, 4, 2>;
using Matrix43f = Matrix<float, 4, 3>;
using Matrix44f = Matrix<float, 4, 4>;

using Matrix12d = Matrix<double, 1, 2>;
using Matrix13d = Matrix<double, 1, 3>;
using Matrix14d = Matrix<double, 1, 4>;
using Matrix21d = Matrix<double, 2, 1>;
using Matrix22d = Matrix<double, 2, 2>;
using Matrix23d = Matrix<double, 2, 3>;
using Matrix24d = Matrix<double, 2, 4>;
using Matrix31d = Matrix<double, 3, 1>;
using Matrix32d = Matrix<double, 3, 2>;
using Matrix33d = Matrix<double, 3, 3>;
using Matrix34d = Matrix<double, 3, 4>;
using Matrix41d = Matrix<double, 4, 1>;
using Matrix42d = Matrix<double, 4, 2>;
using Matrix43d = Matrix<double, 4, 3>;
using Matrix44d = Matrix<double, 4, 4>;

/// @} math_matrix

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
    data[i * cols + i] = value[i];
}


//! matrix out-of-class operations

template<typename T, int m, int n>
static inline
Matrix<T, m, n>&
operator *= (Matrix<T, m, n>& mat, T val) {
  // TODO: use MatrixAssigner or raw access?
//  MatrixAssigner::assign(mat*val, mat);
  for(int i=0; i<mat.size; ++i)
    mat[i] *= val;
  return mat;
}

template<typename T, int m, int n>
static inline
Matrix<T, m, n>&
operator /= (Matrix<T, m, n>& mat, T val) {
  for(int i=0; i<mat.size; ++i)
    mat[i] /= val;
  return mat;
}

template<typename E, typename T, int m, int n>
static inline
Matrix<T, m, n>&
operator += (Matrix<T, m, n>& mat, const MatrixBase<E>& expr) {
  for(int i=0; i<mat.size; ++i)
    mat[i] += expr[i];
  return mat;
}

template<typename E, typename T, int m, int n>
static inline
Matrix<T, m, n>&
operator -= (Matrix<T, m, n>& mat, const MatrixBase<E>& expr) {
  for(int i=0; i<mat.size; ++i)
    mat[i] -= expr[i];
  return mat;
}



}

# endif //VCCC_MATH_MATRIX_MATRIX_HPP
