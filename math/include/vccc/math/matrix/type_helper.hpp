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
      std::conditional_t<traits<T>::option & flag_helper,
                         const std::remove_reference_t<T>,
                         const T&>;
  using non_const_type =
      std::conditional_t<traits<T>::option & flag_helper,
                         std::remove_reference_t<T>,
                         T&>;
};

template<typename T>
using hold_type_selector_t = typename hold_type_selector<T>::type;

template<typename T>
struct is_alias_safe {

};

}} // namespace internal::math
} // namespace vccc


# endif //VCCC_MATH_MATRIX_TYPE_HELPER_HPP_
