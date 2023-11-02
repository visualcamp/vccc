# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_VOID_T_HPP
# define VCCC_TYPE_TRAITS_VOID_T_HPP
#
# include <type_traits>

namespace vccc {

/// @addtogroup type_traits
/// @{
/// @addtogroup type_traits_void_t__class__Miscellaneous_transformations void_t
/// @brief void variadic alias template
/// @{

template<typename ...>
using void_t = void;

/// @}
/// @}

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_VOID_T_HPP
