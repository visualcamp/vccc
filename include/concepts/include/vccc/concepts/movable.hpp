//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_MOVABLE_HPP_
#define VCCC_CONCEPTS_MOVABLE_HPP_

#include <type_traits>

#include "vccc/type_traits/conjunction.hpp"
#include "vccc/concepts/assignable_from.hpp"
#include "vccc/concepts/move_constructible.hpp"
#include "vccc/type_traits/is_swappable.hpp"

namespace vccc {
namespace concepts {

template<typename T>
using movable = conjunction<
    std::is_object<T>,
    move_constructible<T>,
    assignable_from<T&, T>
//    , swappable<T> TODO
    >;

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_MOVABLE_HPP_
