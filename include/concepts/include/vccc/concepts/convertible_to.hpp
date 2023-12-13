//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_CONVERTIBLE_TO_HPP_
#define VCCC_CONCEPTS_CONVERTIBLE_TO_HPP_

#include <type_traits>

#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace concepts {

template<typename From, typename To>
using convertible_to = conjunction<std::is_convertible<From, To>, std::is_constructible<To, From>>;

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_CONVERTIBLE_TO_HPP_
