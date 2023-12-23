//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_SAME_AS_HPP_
#define VCCC_CONCEPTS_SAME_AS_HPP_

#include <type_traits>

namespace vccc {
namespace concepts {

/// @addtogroup concepts
/// @{


/**
 * \brief 	specifies that a type is the same as another type
 *
 * Subsume is impossible without using actual concept
 */
template<typename T, typename U>
struct same_as : std::is_same<T, U> {};

/// @}

} // namespace concepts
} // namespace vccc

#endif // VCCC_CONCEPTS_SAME_AS_HPP_
