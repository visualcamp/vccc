# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_STD_VARIADIC_HPP
# define VCCC_TYPE_SUPPORT_STD_VARIADIC_HPP
#
# include <cstdlib>

namespace vccc {

/**
@addtogroup type_support
@{
*/

/**
 * @brief Index-based value accessor
 * @tparam i
 * @tparam Args
 * @param args
 * @return
 */
template<std::size_t i,
         typename ...Args,
         std::enable_if_t<(i < sizeof...(Args)), int> = 0>
inline
decltype(auto)
variadic_at(Args&&... args){
  return std::get<i>(std::forward_as_tuple(std::forward<Args>(args)...));
}

//! @} type_support

} // namespace vccc

# endif // VCCC_TYPE_SUPPORT_STD_VARIADIC_HPP
