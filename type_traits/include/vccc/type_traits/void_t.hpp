# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_VOID_T_HPP
# define VCCC_TYPE_TRAITS_VOID_T_HPP
#
# include <type_traits>

namespace vccc{

/**
@addtogroup type_traits
@{
*/

template<typename ...>
using void_t = void;

//! @}


}

# endif //VCCC_TYPE_TRAITS_VOID_T_HPP
