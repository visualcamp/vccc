//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_VIEW_HPP_
#define VCCC_RANGES_VIEW_HPP_

#include "vccc/__concepts/movable.hpp"
#include "vccc/__ranges/enable_view.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{

/**
@brief specifies that a range is a view, that is, it has constant time copy/move/assignment

The `%view` concept specifies the requirements of a \ref ranges::range "range" type that has suitable semantic
properties for use in constructing range adaptor pipelines.
 */
template<typename T>
struct view
    : conjunction<
          range<T>,
          movable<T>,
          enable_view<T>
      > {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEW_HPP_
