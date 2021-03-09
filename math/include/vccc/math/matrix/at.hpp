# /*
#  * Created by YongGyu Lee on 2020/02/05.
#  */
#
# ifndef VCCC_MATH_MATRIX_GET_HPP_
# define VCCC_MATH_MATRIX_GET_HPP_
#
# include "vccc/math/matrix/mat_expression.hpp"

namespace vccc {

template<std::size_t i, typename E>
constexpr decltype(auto) at(const MatExpression<E>& mat) { return mat(i); }

template<std::size_t i, typename E>
constexpr decltype(auto) at(MatExpression<E>& mat) { return mat(i); }

template<std::size_t i, typename E>
constexpr decltype(auto) at(MatExpression<E>&& mat) { return mat(i); }

template<std::size_t i, std::size_t j, typename E>
constexpr decltype(auto) at(const MatExpression<E>& mat) { return mat(i, j); }

template<std::size_t i, std::size_t j, typename E>
constexpr decltype(auto) at(MatExpression<E>& mat) { return mat(i, j); }

template<std::size_t i, std::size_t j, typename E>
constexpr decltype(auto) at(MatExpression<E>&& mat) { return mat(i, j); }

}

# endif //VCCC_MATH_MATRIX_GET_HPP_
