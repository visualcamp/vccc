//
// Created by yonggyulee on 2024/02/08.
//

#ifndef VCCC_TYPE_TRAITS_IS_CLASS_OR_ENUM_HPP
#define VCCC_TYPE_TRAITS_IS_CLASS_OR_ENUM_HPP

#include <type_traits>

#include "vccc/__type_traits/disjunction.hpp"

namespace vccc {

//! @addtogroup type_traits
//! @{
//! @addtogroup type_traits_is_class_or_enum__class__Type_properties is_class_or_enum
//! @{

template<class T>
struct is_class_or_enum : disjunction<std::is_class<T>, std::is_enum<T>> {};

/// @}
/// @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_IS_CLASS_OR_ENUM_HPP
