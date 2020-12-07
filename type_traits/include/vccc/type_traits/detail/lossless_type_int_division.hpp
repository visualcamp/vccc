//
// Created by YongGyu Lee on 2020/12/07.
//

#ifndef VCCC_TYPE_TRAITS_DETAIL_LOSSLESS_TYPE_INT_DIVISION_HPP
#define VCCC_TYPE_TRAITS_DETAIL_LOSSLESS_TYPE_INT_DIVISION_HPP

#include <type_traits>
#include <algorithm>

namespace vc{

namespace detail{

// left for specialization
template<typename...> struct lossless_type_int_division;

template<typename T1, typename T2>
struct lossless_type_int_division<T1, T2> {
  using type = std::conditional_t<std::max(sizeof(T1), sizeof(T2)) <= 4, float, double>;
};

template<typename T1, typename T2>
using lossless_type_int_division_t = typename lossless_type_int_division<T1, T2>::type;

}

}

#endif //VCCC_TYPE_TRAITS_DETAIL_LOSSLESS_TYPE_INT_DIVISION_HPP
