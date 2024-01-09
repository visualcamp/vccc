# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_HPP
# define VCCC_MATH_MATRIX_MATRIX_HPP
#
# include <cstddef>
# include <tuple>
# include <utility>
#
# include "vccc/__math/matrix/matrix_base.hpp"

namespace vccc {

namespace internal {
namespace math {

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

struct matrix_ctor_all_t {};
struct matrix_ctor_diag_t {};
struct matrix_ctor_indexable_t {};

constexpr matrix_ctor_all_t matrix_ctor_all;
constexpr matrix_ctor_diag_t matrix_ctor_diag;
constexpr matrix_ctor_indexable_t matrix_ctor_indexable;

} // namespace math
} // namespace internal

/// @addtogroup math_matrix
/// @{

/**
 * @brief A class that represents 2D matrix
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

  template<typename U, int p, int q>
  friend class Matrix;

  /**
   * @brief Create a zero matrix
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

  /**
   * @brief Construct from an array.
   *
   * If the size of an array is smaller than the size of the matrix,
   * remaining elements is initialized to 0
   *
   * This constructor does not participate in overload resolution if the
   * size of an array is greater than the size of the matrix.
   *
   * @tparam N
   * @param arr
   */
  template<std::size_t N, std::enable_if_t<(N <= m * n), int> = 0>
  constexpr explicit Matrix(const T(& arr)[N])
    : Matrix(internal::math::matrix_ctor_indexable, arr, std::make_index_sequence<N>{}, std::true_type{}) {}

  /**
   * @brief Construct from matrix expression
   *
   * @tparam E
   * @param expr
   */
  template<typename E, std::enable_if_t<
      internal::math::is_same_size<matrix_type, E>::value &&
      internal::math::is_same_type<matrix_type, E>::value, int> = 0>
  constexpr Matrix(const MatrixBase<E>& expr)
      : Matrix(internal::math::matrix_ctor_indexable, expr, std::make_index_sequence<size>{}, internal::math::is_alias_safe_t<E>{}) {}

  /**
   * @brief Assign from matrix expression
   *
   * @tparam E
   * @param expr
   * @return *this
   */
  template<typename E, std::enable_if_t<
      internal::math::is_same_size<matrix_type, E>::value &&
      internal::math::is_same_type<matrix_type, E>::value, int> = 0>
  constexpr Matrix& operator=(const MatrixBase<E>& expr) {
    Assign(expr, internal::math::is_alias_safe<E>{});
    return *this;
  }

  /**
   * @brief Create a matrix with a different type
   *
   * @tparam U New type
   * @return Matrix<U, m, n>
   */
  template<typename U, std::enable_if_t<std::is_constructible<U, const value_type&>::value, int> = 0>
  constexpr Matrix<U, m, n> AsType() const {
    return Matrix<U, m, n>(internal::math::matrix_ctor_indexable, *this, std::make_index_sequence<m*n>{}, std::true_type{});
  }

  /**
   * @brief Create a matrix with a different shape but same size
   *
   * @tparam rows New rows
   * @tparam cols New columns
   * @return
   */
  template<int rows, int cols = (m * n) / rows, std::enable_if_t<m * n == rows * cols, int> = 0>
  constexpr Matrix<T, rows, cols> Reshape() const {
    return Matrix<T, rows, cols>(internal::math::matrix_ctor_indexable, *this, std::make_index_sequence<m*n>{}, std::true_type{});
  }

//  inline MatrixProxyNocopy<matrix_type> noAlias() {
//    return MatrixProxyNocopy<matrix_type>(*this);
//  }

  /**
   * @brief Create all-value matrix
   */
  constexpr static Matrix all(T value) {
    return Matrix(internal::math::matrix_ctor_all, value, std::make_index_sequence<size>{});
  }

  /**
   * @brief Create zero matrix
   */
  constexpr static Matrix zeros() {
    return all(T(0));
  }

  /**
   * @brief Create all-ones matrix
   */
  constexpr static Matrix ones() {
    return all(T(1));
  }

  /**
   * @brief Create a diagonal matrix
   */
  constexpr static Matrix diag(const diag_type& value) {
    return Matrix(internal::math::matrix_ctor_diag, value);
  }

  /**
   * @brief Create an identity matrix
   */
  template<typename Dummy = void, std::enable_if_t<
      std::is_void<Dummy>::value &&
      (n == m), int> = 0>
  constexpr static Matrix eye() {
    return Matrix(internal::math::matrix_ctor_all, internal::math::matrix_ctor_diag, 1);
  }

  /**
   * @brief Access element `i`th element from the beginning as if the Matrix is 1-D
   * The behavior is undefined if `i` >= `size`.
   *
   * @param i
   * @return Element at the given index
   */
  constexpr       value_type& operator[](std::size_t i)       noexcept { return data[i]; }
  constexpr const value_type& operator[](std::size_t i) const noexcept { return data[i]; }

  /**
   * @brief Access element `i`th element from the beginning as if the Matrix is 1-D
   * The behavior is undefined if `i` >= `size`.
   *
   *
   * @param i
   * @return Element at the given index
   */
  constexpr       value_type& operator()(std::size_t i)       noexcept { return data[i]; }
  constexpr const value_type& operator()(std::size_t i) const noexcept { return data[i]; }

  /**
   * @brief Access element at (`i`, `j`)
   * The behavior is undefined if `i` * `j` > `size`
   *
   * @param i
   * @param j
   * @return Element at the given index
   */
  constexpr       value_type& operator()(std::size_t i, std::size_t j)       noexcept { return data[i * matrix_type::cols + j]; }
  constexpr const value_type& operator()(std::size_t i, std::size_t j) const noexcept { return data[i * matrix_type::cols + j]; }

 private:
  template<typename U, std::size_t... I>
  constexpr Matrix(internal::math::matrix_ctor_indexable_t, const U& v, std::index_sequence<I...>, std::true_type)
      : data{((void)I, static_cast<value_type>(v[I]))...} {}

  // TODO: Optimize
  template<typename U, std::size_t... I>
  constexpr Matrix(internal::math::matrix_ctor_indexable_t, const U& v, std::index_sequence<I...>, std::false_type)
      : data{} {
    auto copy = this->derived();
    for(int i=0; i<std::index_sequence<I...>::size(); ++i)
      copy[i] = v[i];
    for(int i=0; i<std::index_sequence<I...>::size(); ++i)
      (*this)[i] = std::move(copy[i]);
  }

  template<std::size_t... I>
  constexpr Matrix(internal::math::matrix_ctor_all_t, const T& value, std::index_sequence<I...>)
      : data{((void)I, value)...} {}

  constexpr Matrix(internal::math::matrix_ctor_diag_t, const diag_type& value) : data{} {
    for (int i = 0; i < shortdim; ++i)
      data[i * cols + i] = value[i];
  }

  constexpr Matrix(internal::math::matrix_ctor_all_t, internal::math::matrix_ctor_diag_t, const T& value) : data{} {
    for (int i = 0; i < shortdim; ++i)
      data[i * cols + i] = value;
  }

  template<typename E>
  constexpr void Assign(const MatrixBase<E>& expr, std::true_type) {
    for(int i=0; i<size; ++i)
      (*this)[i] = expr[i];
  }

  // TODO: Optimize
  template<typename E>
  constexpr void Assign(const MatrixBase<E>& expr, std::false_type) {
    auto copy = this->derived();
    for(int i=0; i<matrix_type::size; ++i)
      copy[i] = expr[i];
    for(int i=0; i<matrix_type::size; ++i)
      (*this)[i] = std::move(copy[i]);
  }

  T data[m * n];
};

//! @cond IGNORED
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

//! @endcond IGNORED

// matrix out-of-class operations

template<typename T, int m, int n>
static inline
Matrix<T, m, n>&
operator *= (Matrix<T, m, n>& mat, T val) {
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

/// @} math_matrix

} // namespace vccc

template<typename T, int m, int n>
struct std::tuple_size<vccc::Matrix<T, m, n>>
  : std::integral_constant<std::size_t, m * n> {};

namespace std {

template<std::size_t I, typename T, int m, int n>
constexpr inline std::enable_if_t<(I < m * n), typename vccc::Matrix<T, m, n>::value_type&>
get(vccc::Matrix<T, m, n>& matrix) {
  return matrix[I];
}

template<std::size_t I, typename T, int m, int n>
constexpr inline std::enable_if_t<(I < m * n), const typename vccc::Matrix<T, m, n>::value_type&>
get(const vccc::Matrix<T, m, n>& matrix) {
  return matrix[I];
}

template<std::size_t I, typename T, int m, int n>
constexpr inline std::enable_if_t<(I < m * n), typename vccc::Matrix<T, m, n>::value_type&&>
get(vccc::Matrix<T, m, n>&& matrix) {
  return std::move(matrix[I]);
}

template<std::size_t I, typename T, int m, int n>
constexpr inline std::enable_if_t<(I < m * n), const typename vccc::Matrix<T, m, n>::value_type&&>
get(const vccc::Matrix<T, m, n>&& matrix) {
  return std::move(matrix[I]);
}

} // namespace std

# endif // VCCC_MATH_MATRIX_MATRIX_HPP
