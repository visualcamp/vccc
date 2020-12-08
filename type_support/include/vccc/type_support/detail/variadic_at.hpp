//
// Created by YongGyu Lee on 2020/12/08.
//

#ifndef VCCC_TYPE_SUPPORT_DETAIL_VARIADIC_AT_HPP
#define VCCC_TYPE_SUPPORT_DETAIL_VARIADIC_AT_HPP

#include <cstdlib>

namespace vc{

template<std::size_t i, typename ...Args,
         std::enable_if_t<(i < sizeof...(Args)), int> = 0>
constexpr
decltype(auto)
variadic_at(Args&&... args){
  return std::get<i>(std::forward_as_tuple(std::forward<Args>(args)...));
}

}

#endif //VCCC_TYPE_SUPPORT_DETAIL_VARIADIC_AT_HPP
