//
// Created by YongGyu Lee on 1/18/24.
//

#ifndef VCCC_CONCEPTS_DIFFERENT_FROM_HPP
#define VCCC_CONCEPTS_DIFFERENT_FROM_HPP

#include "vccc/__concepts/same_as.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

template<typename Test, typename From>
struct different_from : negation< same_as<remove_cvref_t<Test>, remove_cvref_t<From>> > {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_DIFFERENT_FROM_HPP
