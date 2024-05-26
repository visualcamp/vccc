//
// Created by yonggyulee on 2023/10/27.
//

#ifndef VCCC_TYPE_TRAITS_IS_REFERENCEABLE_HPP
#define VCCC_TYPE_TRAITS_IS_REFERENCEABLE_HPP

#include <type_traits>

#include "vccc/__type_traits/void_t.hpp"

namespace vccc {

/// @addtogroup type_traits
/// @{

template<typename T, typename = void>
struct is_referencable : std::false_type {};

template<typename T>
struct is_referencable<T, void_t<T&>> : std::true_type {};

/// @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_IS_REFERENCEABLE_HPP
