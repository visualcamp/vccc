//
// Created by yonggyulee on 2024/01/29.
//

#ifndef VCCC_RANGES_VIEWS_DROP_HPP
#define VCCC_RANGES_VIEWS_DROP_HPP

#include <algorithm>
#include <cstddef>
#include <type_traits>

#include "vccc/__concepts/different_from.hpp"
#include "vccc/__core/decay_copy.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/subrange.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/drop_view.hpp"
#include "vccc/__ranges/views/empty_view.hpp"
#include "vccc/__ranges/views/iota_view.hpp"
#include "vccc/__ranges/views/repeat.hpp"
#include "vccc/__ranges/views/repeat_view.hpp"
#include "vccc/span.hpp"
#include "vccc/string_view.hpp"
#include "vccc/__type_traits/detail/return_category.hpp"
#include "vccc/__type_traits/is_specialization.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

template<typename D>
struct drop_adaptor_closure : range_adaptor_closure<drop_adaptor_closure<D>> {
  template<typename U, std::enable_if_t<conjunction<
    different_from<U, drop_adaptor_closure>,
    std::is_constructible<D, U&&>
  >::value, int> = 0>
  constexpr explicit drop_adaptor_closure(U&& count) : count_(std::forward<U>(count)) {}

  template<typename R, std::enable_if_t<range<R>::value, int> = 0>
  constexpr drop_view<all_t<R>>
  operator()(R&& r) const {
    return drop_view<all_t<R>>(std::forward<R>(r), count_);
  }

 private:
  D count_;
};

using vccc::detail::return_category;

struct drop_niebloid {
 private:
  // empty_view - 1
  template<typename R, typename T, typename D, bool = is_specialization<T, empty_view>::value /* true */>
  struct return_category_empty_view : std::true_type {
    using category = return_category<1, decltype(vccc_decay_copy(std::declval<R>()))>;
  };
  template<typename R, typename T, typename D>
  struct return_category_empty_view<R, T, D, false> : std::false_type {
    using category = return_category<0>;
  };
  template<typename R, typename D, typename RT>
  constexpr RT operator()(R&& r, D, return_category<1, RT>) const {
    return std::forward<R>(r);
  }

  // subrange - 2 / 3
  template<typename T>
  struct is_subrange : std::false_type {};
  template<typename I, typename S, subrange_kind K>
  struct is_subrange<subrange<I, S, K>> : std::true_type {};

  template<typename T, bool = conjunction<is_subrange<T>, random_access_range<T>, sized_range<T>>::value /* true */>
  struct return_category_subrange : std::true_type {
    using category = std::conditional_t<ranges::detail::is_size_storing_subrange<T>::value,
        return_category<2, subrange<iterator_t<T>, sentinel_t<T>, subrange_kind::sized> >,
        return_category<3, subrange<iterator_t<T>, sentinel_t<T>> >
    >;
  };
  template<typename T>
  struct return_category_subrange<T, false> : std::false_type {
    using category = return_category<0>;
  };

  template<typename R, typename D, typename U>
  constexpr U operator()(R&& e, D f, return_category<2, U>) const {
    auto inc = (std::min<D>)(ranges::distance(e), f);
    return U(
        ranges::begin(std::forward<R>(e)) + inc,
        ranges::end(std::forward<R>(e)),
        static_cast<std::make_unsigned_t<decltype(inc)>>(ranges::distance(e) - inc)
    );
  }

  // span - 3
  template<typename T>
  struct is_span : std::false_type {};
  template<typename T, std::size_t Extent>
  struct is_span<span<T, Extent>> : std::true_type {};

  template<typename T, typename D, bool = is_span<T>::value /* true */>
  struct return_category_span : std::true_type {
    using category = return_category<3, span<typename T::element_type>>;
  };
  template<typename T, typename D>
  struct return_category_span<T, D, false> : std::false_type {
    using category = return_category<0>;
  };

  // basic_string_view - 3
  template<typename T, bool = is_specialization<T, basic_string_view>::value /* true */>
  struct return_category_string_view : std::true_type {
    using category = return_category<3, T>;
  };
  template<typename T>
  struct return_category_string_view<T, false> : std::false_type {
    using category = return_category<0>;
  };

  // iota_view - 3
  template< typename T, bool = is_specialization<T, iota_view>::value /* true */>
  struct return_category_iota_view : std::true_type {
    using category = return_category<3, T>;
  };
  template<typename T>
  struct return_category_iota_view<T, false> : std::false_type {
    using category = return_category<0>;
  };

  template<typename R, typename D, typename U>
  constexpr U operator()(R&& e, D f, return_category<3, U>) const {
    return U(
        ranges::begin(std::forward<R>(e)) + (std::min<D>)(ranges::distance(std::forward<R>(e)), f),
        ranges::end(std::forward<R>(e))
    );
  }

  // repeat_view - 4
  template<typename T>
  struct return_category_repeat_view : is_specialization<T, repeat_view> {
    using category = return_category<(is_specialization<T, repeat_view>::value ? 4 : 0)>;
  };
  template<typename R, typename D>
  constexpr auto operator()(R&& e, D f, return_category<4>, std::true_type /* sized_range */) const {
    return views::repeat(*e, ranges::distance(e) - (std::min<D>)(ranges::distance(e), f));
  }
  template<typename R, typename D>
  constexpr auto operator()(R&& e, D, return_category<4>, std::false_type /* sized_range */) const {
    return vccc_decay_copy(e);
  }
  template<typename R, typename D>
  constexpr auto operator()(R&& e, D f, return_category<4>) const {
    return (*this)(std::forward<R>(e), f, sized_range<remove_cvref_t<R>>{});
  }

  template<typename R, typename D>
  constexpr drop_view<all_t<R>> operator()(R&& r, D f, return_category<0>) const {
      return drop_view<all_t<R>>(std::forward<R>(r), f);
  }

  template<typename R, typename T, typename D>
  using category =
      std::conditional_t<
          return_category_empty_view<R, T, D>::value, typename return_category_empty_view<R, T, D>::category, // 1
      std::conditional_t<
          return_category_span<T, D>::value, typename return_category_span<T, D>::category, // 3
      std::conditional_t<
          return_category_string_view<T>::value, typename return_category_string_view<T>::category, // 3
      std::conditional_t<
          return_category_subrange<T>::value, typename return_category_subrange<T>::category, // 2 or 3
      std::conditional_t<
          return_category_iota_view<T>::value, typename return_category_iota_view<T>::category, // 3
      std::conditional_t<
          return_category_repeat_view<T>::value, typename return_category_iota_view<T>::category, // 4
          return_category<0>
      >>>>>>;

 public:
  template<typename R, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto
  operator()(R&& r, range_difference_t<R> count) const {
    using T = remove_cvref_t<decltype((r))>;
    using D = range_difference_t<decltype((r))>;
    return (*this)(std::forward<R>(r), count, category<R&&, T, D>{});
  }

  template<typename DifferenceType>
  constexpr drop_adaptor_closure<std::remove_reference_t<DifferenceType>>
  operator()(DifferenceType&& count) const {
    return drop_adaptor_closure<std::remove_reference_t<DifferenceType>>(std::forward<DifferenceType>(count));
  }
};

} // namespace detail

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::drop_niebloid drop{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_DROP_HPP
