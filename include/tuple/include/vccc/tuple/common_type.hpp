//
// Created by yonggyulee on 2023/10/27.
//

#ifndef VCCC_TUPLE_COMMON_TYPE_HPP
#define VCCC_TUPLE_COMMON_TYPE_HPP

#include <tuple>
#include <type_traits>

#include "vccc/tuple/tuple_like.hpp"
#include "vccc/type_traits/common_type.hpp"

namespace vccc {

namespace internal {

template<typename T, typename U,
    bool v = (std::tuple_size<T>::value == std::tuple_size<U>::value)>
struct common_type_impl;

template<typename ...Ts, typename ...Us>
struct common_type_impl<std::tuple<Ts...>, std::tuple<Us...>, true> {
  using type = std::tuple<std::common_type_t<Ts, Us>...>;
};

} // namespace internal

template<typename ...Ts, typename ...Us>
struct common_type<std::tuple<Ts...>, std::tuple<Us...>>
    : internal::common_type_impl<std::tuple<Ts...>, std::tuple<Us...>> {};

} // namespace vccc

#endif // VCCC_TUPLE_COMMON_TYPE_HPP
