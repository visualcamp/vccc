//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_DERIVED_FROM_HPP_
#define VCCC_CONCEPTS_DERIVED_FROM_HPP_

#include <type_traits>

#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace concepts {

/// @addtogroup concepts
/// @{

template<typename Derived, typename Base>
struct derived_from
    : conjunction<
        std::is_base_of<Base, Derived>,
        std::is_convertible<const volatile Derived*, const volatile Base*>
      > {};

/// @}

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_DERIVED_FROM_HPP_
