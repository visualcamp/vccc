//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_TYPE_TRAITS_DETAIL_RETURN_CATEGORY_HPP_
#define VCCC_TYPE_TRAITS_DETAIL_RETURN_CATEGORY_HPP_

#include <type_traits>

namespace vccc {
namespace detail {

template<int v, typename...>
struct return_category : std::integral_constant<int, v> {};

template<int v, typename R>
struct return_category<v, R> : std::integral_constant<int, v> {
  using return_type = R;
};


} // namespace vccc
} // namespace detail

#endif // VCCC_TYPE_TRAITS_DETAIL_RETURN_CATEGORY_HPP_
