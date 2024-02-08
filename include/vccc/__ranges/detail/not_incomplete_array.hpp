//
// Created by YongGyu Lee on 2/8/24.
//

#ifndef VCCC_RANGES_DETAIL_NOT_INCOMPLETE_ARRAY_HPP_
#define VCCC_RANGES_DETAIL_NOT_INCOMPLETE_ARRAY_HPP_

#include <type_traits>

#include "vccc/__type_traits/is_complete.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace detail {

template<typename T, bool = std::is_array<remove_cvref_t<T>>::value /* false */>
struct not_incomplete_array : std::true_type {};

template<typename T>
struct not_incomplete_array<T, true>
    : is_complete<std::remove_all_extents_t<std::remove_reference_t<T>>> {};

} // namespace detail
} // namespace vccc

#endif // VCCC_RANGES_DETAIL_NOT_INCOMPLETE_ARRAY_HPP_
