//
// Created by YongGyu Lee on 2020/12/07.
//

#ifndef VCCC_TYPE_TRAITS_VOID_T_HPP
#define VCCC_TYPE_TRAITS_VOID_T_HPP

#include <type_traits>

namespace vc{

#if __cplusplus <= 201402L
template<typename ...>
using void_t = void;
#else
template<typename ...Args> using void_t = ::std::void_t<Args...>;
#endif


}

#endif //VCCC_TYPE_TRAITS_VOID_T_HPP
