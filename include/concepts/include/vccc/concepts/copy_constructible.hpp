//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_COPY_CONSTRUCTIBLE_HPP_
#define VCCC_CONCEPTS_COPY_CONSTRUCTIBLE_HPP_

#include "vccc/concepts/move_constructible.hpp"
#include "vccc/concepts/constructible_from.hpp"
#include "vccc/concepts/convertible_to.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace concepts {

template<typename T>
struct copy_constructible
    : conjunction<
        move_constructible<T>,
        constructible_from<T, T&>, convertible_to<T&, T>,
        constructible_from<T, const T&>, convertible_to<const T&, T>,
        constructible_from<T, const T>, convertible_to<const T, T>
      > {};

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_COPY_CONSTRUCTIBLE_HPP_
