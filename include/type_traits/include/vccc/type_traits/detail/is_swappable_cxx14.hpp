//
// Created by yonggyulee on 2023/12/20.
//

#ifndef VCCC_TYPE_TRAITS_DETAIL_IS_SWAPPABLE_CXX14_HPP_
#define VCCC_TYPE_TRAITS_DETAIL_IS_SWAPPABLE_CXX14_HPP_

#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vccc/type_traits/conjunction.hpp"

namespace vccc {

template<typename T1, typename T2>
struct is_swappable<std::pair<T1, T2>> : conjunction<is_swappable<T1>, is_swappable<T2>> {};

template<typename T1, typename T2>
struct is_swappable<const std::pair<T1, T2>> : conjunction<is_swappable<const T1>, is_swappable<const T2>> {};

template<typename... Types>
struct is_swappable<std::tuple<Types...>>
    : conjunction<is_swappable<Types>...> {};

template<typename... Types>
struct is_swappable<const std::tuple<Types...>>
    : conjunction<is_swappable<const Types>...> {};

template<typename T>
struct is_swappable<std::shared_ptr<T>> : std::true_type {};

template<typename T>
struct is_swappable<std::weak_ptr<T>> : std::true_type {};

template<typename T, typename D>
struct is_swappable<std::unique_ptr<T, D>> : is_swappable<D> {};

template<typename R, typename ...Args>
struct is_swappable<std::function<R(Args...)>> : std::true_type {};

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_DETAIL_IS_SWAPPABLE_CXX14_HPP_
