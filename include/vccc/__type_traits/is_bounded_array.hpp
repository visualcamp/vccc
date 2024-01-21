//
// Created by yonggyulee on 2023/09/29.
//

#ifndef VCCC_TYPE_TRAITS_IS_BOUNDED_ARRAY_HPP
#define VCCC_TYPE_TRAITS_IS_BOUNDED_ARRAY_HPP

#include <cstddef>
#include <type_traits>

namespace vccc {

//! @addtogroup type_traits
//! @{
//! @addtogroup type_traits_is_bounded_array__class__Type_properties is_bounded_array
//! @{

template<class T>
struct is_bounded_array : std::false_type {};

template<class T, std::size_t N>
struct is_bounded_array<T[N]> : std::true_type {};

/// @}
/// @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_IS_BOUNDED_ARRAY_HPP
