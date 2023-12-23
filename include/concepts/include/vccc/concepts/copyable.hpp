//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_COPYABLE_HPP_
#define VCCC_CONCEPTS_COPYABLE_HPP_

#include "vccc/concepts/assignable_from.hpp"
#include "vccc/concepts/copy_constructible.hpp"
#include "vccc/concepts/movable.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace concepts {

template<typename T>
struct copyable
    : conjunction<
        copy_constructible<T>,
        movable<T>,
        assignable_from<T&, T&>,
        assignable_from<T&, const T&>,
        assignable_from<T&, const T>
      > {};

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_COPYABLE_HPP_
