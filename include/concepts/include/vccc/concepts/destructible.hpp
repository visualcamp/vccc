//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_DESTRUCTIBLE_HPP_
#define VCCC_CONCEPTS_DESTRUCTIBLE_HPP_

#include <type_traits>

namespace vccc {
namespace concepts {

template<typename T>
using destructible = std::is_nothrow_destructible<T>;

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_DESTRUCTIBLE_HPP_
