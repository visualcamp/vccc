//
// Created by yonggyulee on 2023/09/29.
//

#ifndef VCCC_TYPE_TRAITS_TYPE_IDENTITY_HPP
#define VCCC_TYPE_TRAITS_TYPE_IDENTITY_HPP

namespace vccc {

/// @addtogroup type_traits
/// @{
/// @defgroup type_traits_type_identity__class__Miscellaneous_transformations type_identity
/// @brief returns the type argument unchanged
/// @{

template<typename T>
struct type_identity {
  using type = T;
};

template<typename T>
using type_identity_t = typename type_identity<T>::type;

/// @}
/// @} type_traits

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_TYPE_IDENTITY_HPP
