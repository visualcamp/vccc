//
// Created by yonggyulee on 1/26/24.
//

#ifndef VCCC_ALGORITHM_RANGES_FOLD_LEFT_HPP
#define VCCC_ALGORITHM_RANGES_FOLD_LEFT_HPP

#include <functional>
#include <type_traits>
#include <utility>

#include "vccc/__concepts/assignable_from.hpp"
#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__concepts/invocable.hpp"
#include "vccc/__concepts/movable.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/indirectly_readable.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"
#include "vccc/__type_traits/is_invocable.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename F, typename T, typename I, typename U,
    bool = conjunction<
        movable<T>,
        movable<U>,
        convertible_to<T, U>,
        invocable<F&, U, iter_reference_t<I>>
    >::value /* true */>
struct indirectly_binary_left_foldable_impl
    : assignable_from<U&, invoke_result_t<F&, U, iter_reference_t<I>>> {};
template<typename F, typename T, typename I, typename U>
struct indirectly_binary_left_foldable_impl<F, T, I, U, false> : std::false_type {};

template<typename F, typename T, typename I,
    bool = invocable<F&, T, iter_reference_t<I>>::value /* true */>
struct indirectly_binary_left_foldable_2
    : conjunction<
          convertible_to<
              invoke_result_t<F&, T, iter_reference_t<I>>,
              std::decay_t<invoke_result_t<F&, T, iter_reference_t<I>>>>,
          indirectly_binary_left_foldable_impl<F, T, I, std::decay_t<invoke_result_t<F&, T, iter_reference_t<I>>>>
      >{};
template<typename F, typename T, typename I>
struct indirectly_binary_left_foldable_2<F, T, I, false> : std::false_type {};

template<typename F, typename T, typename I,
    bool = conjunction<
        copy_constructible<F>,
        indirectly_readable<I>,
        has_typename_type<iter_reference<I>>
    >::value /* true */ >
struct indirectly_binary_left_foldable : indirectly_binary_left_foldable_2<F, T, I> {};
template<typename F, typename T, typename I>
struct indirectly_binary_left_foldable<F, T, I, false> : std::false_type {};

struct fold_left_niebloid {
 private:
  template<typename R, typename T, typename F, bool = input_range<R>::value /* false */>
  struct check_range : std::false_type {};
  template<typename R, typename T, typename F>
  struct check_range<R, T, F, true>
      : indirectly_binary_left_foldable<F, T, iterator_t<R>> {};

 public:
  template<typename I, typename S, typename T, typename F, std::enable_if_t<conjunction<
      input_iterator<I>,
      sentinel_for<S, I>,
      indirectly_binary_left_foldable<F, T, I>
  >::value, int> = 0>
  constexpr auto operator()(I first, S last, T init, F f) const {
    using U = std::decay_t<invoke_result_t<F&, T, iter_reference_t<I>>>;

    if (first == last)
      return U(std::move(init));

    U accum = vccc::invoke(f, std::move(init), *first);
    for (++first; first != last; ++first) {
          accum = vccc::invoke(f, std::move(accum), *first);
    }

    return std::move(accum);
  }

  template<typename R, typename T, typename F, std::enable_if_t<check_range<R, T, F>::value, int> = 0>
  constexpr auto operator()(R&& r, T init, F f) const {
    return (*this)(ranges::begin(r), ranges::end(r), std::move(init), std::ref(f));
  }
};

} // namespace detail

/// @addtogroup algorithm
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::fold_left_niebloid fold_left{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_FOLD_LEFT_HPP
