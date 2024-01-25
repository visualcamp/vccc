//
// Created by yonggyulee on 1/24/24.
//

#ifndef VCCC_TYPE_TRAITS_HAS_OPERATOR_ARROW_HPP
#define VCCC_TYPE_TRAITS_HAS_OPERATOR_ARROW_HPP

#include <type_traits>

#include "vccc/__type_traits/void_t.hpp"

namespace vccc {

template<typename T, typename = void>
struct has_operator_arrow : std::false_type {};

template<typename T>
struct has_operator_arrow<T, void_t<decltype(std::declval<T>().operator->())>> : std::true_type {};

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_HAS_OPERATOR_ARROW_HPP
