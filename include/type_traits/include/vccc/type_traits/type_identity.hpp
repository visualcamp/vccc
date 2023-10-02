//
// Created by yonggyulee on 2023/09/29.
//

#ifndef VCCC_TYPE_TRAITS_TYPE_IDENTITY_HPP_
#define VCCC_TYPE_TRAITS_TYPE_IDENTITY_HPP_

namespace vccc {

template<typename T>
struct type_identity {
  using type = T;
};

template<typename T>
using type_identity_t = typename type_identity<T>::type;

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_TYPE_IDENTITY_HPP_
