//
// Created by yonggyulee on 2023/12/30.
//

#ifndef VCCC_RANGES_ENABLE_VIEW_HPP
#define VCCC_RANGES_ENABLE_VIEW_HPP

#include "vccc/concepts/derived_from.hpp"
#include "vccc/concepts/derived_from_single_crtp.hpp"
#include "vccc/ranges/view_base.hpp"
#include "vccc/ranges/view_interface.hpp"
#include "vccc/type_traits/disjunction.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{


/**
@brief The `%enable_view` variable template is used to indicate whether a \ref ranges::range "range" is a view.

<H1>Notes</H1>
`%vccc::ranges::enable_view` is implemented as `struct`(`std::ranges::enable_view` is `inline constexpr bool`).
Specializations must be inherited from `std::integral_constant<bool, v>`.

@sa [std::ranges::enable_view](https://en.cppreference.com/w/cpp/ranges/view)
@sa view_base
@sa view_interface
 */
template<typename T>
struct enable_view : disjunction<
          derived_from<T, view_base>,
          derived_from_single_crtp<T, view_interface>
      > {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_ENABLE_VIEW_HPP
