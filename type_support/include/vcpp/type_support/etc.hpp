//
// Created by YongGyu Lee on 2020/12/02.
//

#ifndef VCPP_TYPE_SUPPORT_ETC_HPP
#define VCPP_TYPE_SUPPORT_ETC_HPP

#include <vector>

namespace vc {

template<typename ...T>
std::vector<T...> reserved_vector(typename std::vector<T...>::size_type size){
  std::vector<T...> vec;
  vec.reserve(size);
  return vec;
}

}

#endif //VCPP_TYPE_SUPPORT_ETC_HPP
