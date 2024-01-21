# /*
#  * Created by YongGyu Lee on 2021/09/03.
#  */
#
# ifndef VCCC_TYPE_TRAITS_BOOL_CONSTANT_HPP
# define VCCC_TYPE_TRAITS_BOOL_CONSTANT_HPP
# 
# include <type_traits>

namespace vccc {

//! @addtogroup type_traits
//! @{
//! @addtogroup type_traits_bool_constant__class__helper_classes bool_constant
//! @brief compile-time constant of specified type with specified value
//! @{

template<bool v>
using bool_constant = std::integral_constant<bool, v>;

template<typename ...> struct always_false : std::false_type {};
template<typename ...> struct always_true : std::true_type {};

//! @}
//! @}

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_BOOL_CONSTANT_HPP
