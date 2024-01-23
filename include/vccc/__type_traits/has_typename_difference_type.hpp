//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_TYPE_TRAITS_HAS_TYPENAME_DIFFERENCE_TYPE_HPP_
#define VCCC_TYPE_TRAITS_HAS_TYPENAME_DIFFERENCE_TYPE_HPP_

#include <type_traits>

#include "vccc/__type_traits/void_t.hpp"

namespace vccc {

template<typename T, typename = void>
struct has_typename_difference_type : std::false_type {};

template<typename T>
struct has_typename_difference_type<T, void_t<typename T::difference_type>> : std::true_type {};

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_HAS_TYPENAME_DIFFERENCE_TYPE_HPP_
