//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_DEREFERENCEABLE_HPP_
#define VCCC_CONCEPTS_DEREFERENCEABLE_HPP_

#include <type_traits>

#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace concepts {

template<typename T, typename = void>
struct dereferenceable : std::false_type {};

template<typename T>
struct dereferenceable<T, void_t<decltype(*std::declval<T&>())>> : std::true_type {};

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_DEREFERENCEABLE_HPP_
