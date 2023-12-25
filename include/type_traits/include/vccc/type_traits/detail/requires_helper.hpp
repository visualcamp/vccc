//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_TYPE_TRAITS_DETAIL_REQUIRES_HELPER_HPP_
#define VCCC_TYPE_TRAITS_DETAIL_REQUIRES_HELPER_HPP_

namespace vccc {
namespace detail {

template<bool v>
struct requires_helper {
  static constexpr bool require = v;
};

using requires_fail = requires_helper<false>;
using requires_pass = requires_helper<true>;

template<typename T>
struct require : std::integral_constant<bool, T::require> {};

} // namespace vccc
} // namespace detail

#endif // VCCC_TYPE_TRAITS_DETAIL_REQUIRES_HELPER_HPP_
