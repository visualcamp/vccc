//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_MOVE_CONSTRUCTIBLE_HPP_
#define VCCC_CONCEPTS_MOVE_CONSTRUCTIBLE_HPP_

#include "vccc/concepts/constructible_from.hpp"
#include "vccc/concepts/convertible_to.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

template<typename T>
struct move_constructible : conjunction<constructible_from<T, T>, convertible_to<T, T>> {};

/// @}


} // namespace vccc

#endif // VCCC_CONCEPTS_MOVE_CONSTRUCTIBLE_HPP_
