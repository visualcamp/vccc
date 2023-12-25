//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_EQUALITY_COMPARABLE_HPP_
#define VCCC_CONCEPTS_EQUALITY_COMPARABLE_HPP_

#include "vccc/concepts/weakly_equality_comparable_with.hpp"

namespace vccc {
namespace concepts {

template<typename T>
struct equality_comparable : weakly_equality_comparable_with<T, T> {};

} // namespace vccc
} // namespace concepts

#endif // VCCC_CONCEPTS_EQUALITY_COMPARABLE_HPP_
