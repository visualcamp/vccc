//
// Created by YongGyu Lee on 2020/12/07.
//

#ifndef VCCC_TYPE_TRAITS_VTYPE_T_HPP
#define VCCC_TYPE_TRAITS_VTYPE_T_HPP

namespace vc{

template<typename T>
using vtype_t = typename T::value_type;

}

#endif //VCCC_TYPE_TRAITS_VTYPE_T_HPP
