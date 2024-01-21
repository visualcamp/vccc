//
// Created by yonggyulee on 2023/09/29.
//

#ifndef VCCC_TYPE_TRAITS_IS_UNBOUNDED_ARRAY_HPP
#define VCCC_TYPE_TRAITS_IS_UNBOUNDED_ARRAY_HPP

#include <cstddef>
#include <type_traits>

namespace vccc {

/// @addtogroup type_traits
/// @{
/// @addtogroup type_traits_is_unbounded_array__class__Type_properties is_unbounded_array
/// @{

template<class T>
struct is_unbounded_array: std::false_type {};

template<class T>
struct is_unbounded_array<T[]> : std::true_type {};

/// @}
/// @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_IS_UNBOUNDED_ARRAY_HPP
