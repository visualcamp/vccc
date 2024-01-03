//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_BORROWED_RANGE_HPP_
#define VCCC_RANGES_BORROWED_RANGE_HPP_

#include <type_traits>

#include "vccc/ranges/__forward_declare.hpp"
#include "vccc/ranges/enable_borrowed_range.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/disjunction.hpp"
#include "vccc/type_traits/remove_cvref.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{

template<typename R>
struct borrowed_range
    : conjunction<
        ranges::range<R>,
        disjunction<
          std::is_lvalue_reference<R>,
          ranges::enable_borrowed_range<remove_cvref_t<R>>
        >
      > {};

/// @}

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_BORROWED_RANGE_HPP_
