# /*
#  * Created by YongGyu Lee on 2020/02/23.
#  */
#
# ifndef VCCC_MATH_MATRIX_TYPE_HELPER_HPP_
# define VCCC_MATH_MATRIX_TYPE_HELPER_HPP_
#
# include <type_traits>
#
# include "vccc/math/matrix/forward_declare.hpp"

namespace vccc {

namespace internal{ namespace math{

template<typename T>
struct hold_type_selector {
  using type =
      std::conditional_t<bool(traits<T>::option & Flag::kReferenceUnsafe),
                         const std::remove_reference_t<T>,
                         const T&>;
  using non_const_type =
      std::conditional_t<bool(traits<T>::option & Flag::kReferenceUnsafe),
                         std::remove_reference_t<T>,
                         T&>;
};

template<typename T>
using hold_type_selector_t = typename hold_type_selector<T>::type;

template<typename T>
struct is_alias_safe : std::integral_constant<bool, !(traits<T>::option & Flag::kAliasUnsafe)> {};

template<typename T> using is_alias_safe_t = typename is_alias_safe<T>::type;
template<typename T> constexpr bool is_alias_safe_v = is_alias_safe<T>::value;


template<typename T>
struct is_concrete_matrix : std::false_type {};

template<typename T, int m, int n>
struct is_concrete_matrix<Matrix<T, m, n>> : std::true_type {};

template<typename T>
using is_concrete_matrix_t = typename is_concrete_matrix<T>::type;

template<typename T>
constexpr bool is_concrete_matrix_v = is_concrete_matrix<T>::value;

template<typename TL, typename TR>
struct is_same_size_impl
    : std::integral_constant<bool, ((TL::rows == TR::rows) && (TL::cols == TR::cols))> {};

template<typename A, typename B>
struct is_same_size : is_same_size_impl<traits<A>, traits<B>> {};

}} // namespace internal::math
} // namespace vccc


# endif //VCCC_MATH_MATRIX_TYPE_HELPER_HPP_
