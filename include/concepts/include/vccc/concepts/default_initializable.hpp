//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_DEFAULT_INITIALIZABLE_HPP_
#define VCCC_CONCEPTS_DEFAULT_INITIALIZABLE_HPP_

#include "vccc/concepts/constructible_from.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace concepts {
namespace detail {

template<typename T, typename = void>
struct is_direct_list_initializable : std::false_type {};
template<typename T>
struct is_direct_list_initializable<T, void_t<decltype(T{})>> : std::true_type {};

template<typename T, typename = void>
struct is_default_initializable : std::false_type {};
template<typename T>
struct is_default_initializable<T, void_t<decltype(::new T)>> : std::true_type {};

} // namespace detail

/// @addtogroup concepts
/// @{

template<typename T>
struct default_initializable
    : conjunction<
        constructible_from<T>,
        detail::is_direct_list_initializable<T>,
        detail::is_default_initializable<T>
      > {};

/// @}

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_DEFAULT_INITIALIZABLE_HPP_
