//
// Created by yonggyulee on 1/27/24.
//

#ifndef VCCC_RANGES_VIEWS_COUNTED_HPP_
#define VCCC_RANGES_VIEWS_COUNTED_HPP_

#include <cstddef>
#include <type_traits>

#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/contiguous_iterator.hpp"
#include "vccc/__iterator/counted_iterator.hpp"
#include "vccc/__iterator/default_sentinel_t.hpp"
#include "vccc/__iterator/input_or_output_iterator.hpp"
#include "vccc/__iterator/iter_difference_t.hpp"
#include "vccc/__iterator/random_access_iterator.hpp"
#include "vccc/__memory/to_address.hpp"
#include "vccc/span.hpp"
#include "vccc/__ranges/subrange.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/detail/return_category.hpp"
#include "vccc/__type_traits/detail/tag.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

using vccc::detail::return_category;
using vccc::detail::tag_1;
using vccc::detail::tag_2;
using vccc::detail::tag_else;

struct counted_niebloid {
 private:
  template<typename T, typename DiffType, bool = input_or_output_iterator<T>::value>
  struct check : convertible_to<DiffType, iter_difference_t<T>> {};
  template<typename T, typename DiffType>
  struct check<T, DiffType, false> : std::false_type {};

  template<typename T>
  using tag = vccc::detail::conditional_tag<
      contiguous_iterator<T>,
      random_access_iterator<T>>;

  template<typename D, typename I, typename DiffType>
  constexpr auto call(I&& it, DiffType&& count, tag_1 /* contigious_iterator */) const {
    using Address = decltype(vccc::to_address(std::forward<I>(it)));
    using T = std::remove_reference_t<iter_reference_t<Address>>;
    return span<T>(
        vccc::to_address(std::forward<I>(it)),
        static_cast<std::size_t>(static_cast<D>(std::forward<DiffType>(count))) );
  }

  template<typename D, typename I, typename DiffType>
  constexpr auto call(I&& it, DiffType&& count, tag_2 /* random_access_iterator */) const {
    return ranges::make_subrange(it, it + static_cast<D>(std::forward<DiffType>(count)));
  }

  template<typename D, typename I, typename DiffType>
  constexpr auto call(I&& it, DiffType&& count, tag_else) const {
    return ranges::make_subrange(
        counted_iterator<remove_cvref_t<I>>(
            std::forward<I>(it),
            std::forward<DiffType>(count)),
        default_sentinel
    );
  }

 public:
  template<typename I, typename DiffType, std::enable_if_t<
      check<std::decay_t<I>, DiffType&&>::value, int> = 0>
  constexpr auto operator()(I&& it, DiffType&& count) const {
    using T = std::decay_t<I>;
    using D = iter_difference_t<T>;
    return call<D>(std::forward<I>(it), std::forward<DiffType>(count), tag<T>{});
  }

};

} // namespace detail

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::counted_niebloid counted{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_COUNTED_HPP_
