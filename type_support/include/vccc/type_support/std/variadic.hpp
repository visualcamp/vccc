# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_STD_VARIADIC_HPP
# define VCCC_TYPE_SUPPORT_STD_VARIADIC_HPP
#
# include <cstdlib>

namespace vccc{

/**
@addtogroup type_support_at
@{

@defgroup type_support_at_variadic vccc::at (variadic)
Index-based value accessor
@addtogroup type_support_at_variadic
@{
*/

template<std::size_t i,
         typename ...Args,
         std::enable_if_t<(i < sizeof...(Args)), int> = 0>
constexpr
decltype(auto)
variadic_at(Args&&... args){
  return std::get<i>(std::forward_as_tuple(std::forward<Args>(args)...));
}

//! @} type_support_at_variadic
//! @} type_support_at

}

# endif //VCCC_TYPE_SUPPORT_STD_VARIADIC_HPP
