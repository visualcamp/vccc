//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_MOVABLE_HPP_
#define VCCC_CONCEPTS_MOVABLE_HPP_

#include <type_traits>

#include "vccc/concepts/assignable_from.hpp"
#include "vccc/concepts/move_constructible.hpp"
#include "vccc/concepts/swappable.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_swappable.hpp"

namespace vccc {
namespace concepts {

/// @addtogroup concepts
/// @{

template<typename T>
struct movable : conjunction<
    std::is_object<T>,
    move_constructible<T>,
    assignable_from<T&, T>,
    swappable<T>
    > {};

/// @}

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_MOVABLE_HPP_
