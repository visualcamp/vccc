# /*
#  * Created by YongGyu Lee on 2021/09/03.
#  */
#
# ifndef VCCC_TYPE_TRAITS_INTEGRAL_CONSTANT_H_
# define VCCC_TYPE_TRAITS_INTEGRAL_CONSTANT_H_
# 
# include <type_traits>

namespace vccc {

template<bool v>
using bool_constant = std::integral_constant<bool, v>;

template<typename ...> struct always_false : std::false_type {};
template<typename ...> struct always_true : std::true_type {};

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_INTEGRAL_CONSTANT_H_
