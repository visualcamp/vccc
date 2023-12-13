//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_CONSTRUCTIBLE_FROM_HPP_
#define VCCC_CONCEPTS_CONSTRUCTIBLE_FROM_HPP_

#include "vccc/concepts/destructible.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace concepts {

template<typename T, typename... Args>
using constructible_from = conjunction<
    concepts::destructible<T>,
    std::is_constructible<T, Args...>>;

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_CONSTRUCTIBLE_FROM_HPP_
