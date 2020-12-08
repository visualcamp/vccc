# /*
#   Created by YongGyu Lee on 2020/12/08.
# */
#
# ifndef VCCC_TYPE_SUPPORT_STD_VARIADIC_HPP
# define VCCC_TYPE_SUPPORT_STD_VARIADIC_HPP
#
# include <cstdlib>

namespace vc{

template</* manual */ std::size_t i,
         /* deduce */ typename ...Args,
         /*  cond  */ std::enable_if_t<(i < sizeof...(Args)), int> = 0>
constexpr
decltype(auto)
variadic_at(Args&&... args){
  return std::get<i>(std::forward_as_tuple(std::forward<Args>(args)...));
}

}

#endif //VCCC_TYPE_SUPPORT_STD_VARIADIC_HPP
