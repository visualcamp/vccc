//
// Created by yonggyulee on 2024/01/06.
//

#ifndef VCCC_RANGES_TO_HPP_
#define VCCC_RANGES_TO_HPP_

#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vccc/__algorithm/copy.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__concepts/constructible_from.hpp"
#include "vccc/__concepts/derived_from.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__iterator/iter_key_t.hpp"
#include "vccc/__iterator/iter_val_t.hpp"
#include "vccc/__iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/from_range.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/range_key_t.hpp"
#include "vccc/__ranges/range_mapped_t.hpp"
#include "vccc/__ranges/range_size_t.hpp"
#include "vccc/__ranges/sentinel_t.hpp"
#include "vccc/__ranges/size.hpp"
#include "vccc/__ranges/sized_range.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/views/transform.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/detail/tag.hpp"
#include "vccc/__type_traits/detail/return_category.hpp"
#include "vccc/__type_traits/disjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"
#include "vccc/__type_traits/is_referenceable.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__type_traits/template_arity.hpp"
#include "vccc/__type_traits/void_t.hpp"
#include "vccc/__utility/in_place.hpp"
#include "vccc/__utility/type_sequence.hpp"


namespace vccc {
namespace ranges {
namespace detail {
template<typename T, typename N, typename = void>
struct has_mf_reserve : std::false_type {};
template<typename T, typename N>
struct has_mf_reserve<T, N, void_t<decltype( std::declval<T&>().reserve(std::declval<N>()) )>> : std::true_type {};

template<typename T, typename N, typename = void>
struct has_mf_capacity : std::false_type {};
template<typename T, typename N>
struct has_mf_capacity<T, N, void_t<decltype( std::declval<T&>().capacity() )>>
    : same_as<decltype(std::declval<T&>().capacity()), N> {};

template<typename T, typename N, typename = void>
struct has_mf_max_size : std::false_type {};
template<typename T, typename N>
struct has_mf_max_size<T, N, void_t<decltype( std::declval<T&>().max_size() )>>
    : same_as<decltype(std::declval<T&>().max_size()), N> {};

template<typename Container, bool = ranges::sized_range<Container>::value /* true */>
struct reservable_container
    : conjunction<
          has_mf_reserve<Container, ranges::range_size_t<Container>>,
          has_mf_reserve<Container, ranges::range_size_t<Container>>,
          has_mf_reserve<Container, ranges::range_size_t<Container>>
      >{};
template<typename Container>
struct reservable_container<Container, false> : std::false_type {};

template<typename C, typename R, std::enable_if_t<conjunction<sized_range<R>, reservable_container<C>>::value, int> = 0>
constexpr void try_reserve(C& c, R&& r) {
    c.reserve(static_cast<range_size_t<C>>(ranges::size(r)));
}
template<typename C, typename R, std::enable_if_t<conjunction<sized_range<R>, reservable_container<C>>::value == false, int> = 0>
constexpr void try_reserve(C&, R&&) {}

template<typename T, typename U, typename = void>
struct has_mf_push_back : std::false_type {};
template<typename T, typename U>
struct has_mf_push_back<T, U, void_t<decltype( std::declval<T&>().push_back(std::declval<U>()) )>>
    : std::true_type {};

template<typename T, typename = void>
struct has_mf_end : std::false_type {};
template<typename T>
struct has_mf_end<T, void_t<decltype( std::declval<T&>().end() )>>
    : is_referencable<decltype( std::declval<T&>().end() )> {};

template<typename T, typename U, bool = has_mf_end<T>::value /* false */, typename = void>
struct has_mf_insert : std::false_type {};
template<typename T, typename U>
struct has_mf_insert<T, U, true, void_t<decltype( std::declval<T&>().insert(std::declval<T&>().end(), std::declval<U>()) )>>
    : std::true_type {};

template<typename Container, typename Reference, bool = conjunction<is_referencable<Container>, is_referencable<Reference>>::value>
struct container_insertable
    : disjunction<
          has_mf_push_back<Container, Reference>,
          has_mf_insert<Container, Reference>
      > {};

template<typename Reference, typename C, std::enable_if_t<has_mf_push_back<C, Reference>::value, int> = 0>
constexpr auto container_inserter(C& c) {
    return std::back_inserter(c);
}

template<typename Reference, typename C, std::enable_if_t<has_mf_push_back<C, Reference>::value == false, int> = 0>
constexpr auto container_inserter(C& c) {
    return std::inserter(c, c.end());
}

template<typename R, typename T, bool = input_range<R>::value /* true */>
struct container_compatible_range : convertible_to<range_reference_t<R>, T> {};
template<typename R, typename T>
struct container_compatible_range<R, T, false> : std::false_type {};

using vccc::detail::tag_1;
using vccc::detail::tag_2;
using vccc::detail::tag_3;
using vccc::detail::tag_4;
using vccc::detail::tag_5;

template<bool /* false */, typename C, typename R, typename... Args>
struct ranges_to_1_a_3_impl : std::false_type {};
template<typename C, typename R, typename... Args>
struct ranges_to_1_a_3_impl<true, C, R, Args...>
    : conjunction<
          derived_from<typename cxx20_iterator_traits<iterator_t<R>>::iterator_category, input_iterator_tag>,
          constructible_from<C, iterator_t<R>, sentinel_t<R>, Args...>
      > {};

template<bool /* common_range = false */, typename C, typename R, typename... Args>
struct ranges_to_1_a_3 : std::false_type {};
template<typename C, typename R, typename... Args>
struct ranges_to_1_a_3<true, C, R, Args...>
    : ranges_to_1_a_3_impl<
          vccc::detail::has_typename_iterator_category< cxx20_iterator_traits<iterator_t<R>> >::value,
          C,
          R,
          Args...> {};

template<typename C, typename R, typename... Args>
struct ranges_to_1_a_4
    : conjunction<
          constructible_from<C, Args...>,
          container_insertable<C, range_reference_t<R>>
      > {};

template<typename C, typename R, typename... Args>
struct ranges_to_1_a {
    using tag =
        std::conditional_t<
            constructible_from<C, R, Args...>::value, tag_1,
        std::conditional_t<
            constructible_from<C, from_range_t, R, Args...>::value, tag_2,
        std::conditional_t<
            ranges_to_1_a_3<common_range<R>::value, C, R, Args...>::value, tag_3,
        std::conditional_t<
            ranges_to_1_a_4<C, R, Args...>::value, tag_4,
            tag_else
        >>>>;
};

template<typename C, bool = has_typename_type<range_reference<C>>::value/* false */>
struct ranges_to_1_b {
    using tag = tag_else;
};
template<typename C>
struct ranges_to_1_b<C, true> {
    using tag = std::conditional_t<input_range<range_reference_t<C>>::value, tag_5, tag_else>;
};

template<typename C, typename R, typename... Args>
struct ranges_to_1
    : std::conditional_t<
          disjunction<
              negation<input_range<C>>,
              convertible_to<range_reference_t<R>, range_value_t<C>>
          >::value,
          ranges_to_1_a<C, R, Args...>,
          ranges_to_1_b<C>
      > {};

template<typename C, typename R, typename... Args>
using ranges_to_1_tag = typename ranges_to_1<C, R, Args...>::tag;

template<typename C, typename R, typename... Args>
constexpr C to_1(tag_1, R&& r, Args&&... args) {
    return C(std::forward<R>(r), std::forward<Args>(args)...);
}

template<typename C, typename R, typename... Args>
constexpr C to_1(tag_2, R&& r, Args&&... args) {
    return C(from_range, std::forward<R>(r), std::forward<Args>(args)...);
}

template<typename C, typename R, typename... Args>
constexpr C to_1(tag_3, R&& r, Args&&... args) {
    return C(ranges::begin(r), ranges::end(r), std::forward<Args>(args)...);
}

template<typename C, typename R, typename... Args>
constexpr C to_1(tag_4, R&& r, Args&&... args) {
    C c(std::forward<Args>(args)...);
    detail::try_reserve(c, std::forward<R>(r));
    ranges::copy(std::forward<R>(r), detail::container_inserter<range_reference_t<R>>(c));
    return c;
}

template<typename C, typename R, typename... Args>
constexpr C to_1(tag_5, R&& r, Args&&... args);


// ranges::to< template<typename...> >

template<typename R>
struct ranges_to_input_iterator {
    using iterator_category = input_iterator_tag;
    using value_type = range_value_t<R>;
    using difference_type = std::ptrdiff_t;
    using pointer = std::add_pointer_t<range_reference_t<R>>;
    using reference = range_reference_t<R>;

    reference operator*() const;
    pointer operator->() const;
    ranges_to_input_iterator& operator++();
    ranges_to_input_iterator operator++(int);
    bool operator==(const ranges_to_input_iterator&) const;
};


// template<typename R, bool = is_key_value_range<R>::value /* false */>
// struct range_to_alloc {};
// template<typename R>
// struct range_to_alloc<R, true> {
//   using type = std::pair<std::add_const_t<typename range_value_t<R>::first_type>,
//                          typename range_value_t<R>::second_type>;
// };
// template<typename R>
// using range_to_alloc_t = typename range_to_alloc<R>::type;



// #if __cplusplus >= 201703L
// template<template<typename...> class C, typename R, typename... Args>
// struct DEDUCE_EXPR;
// #else

template<typename R>
struct is_key_value_range
    : conjunction<
        input_range<R>,
        has_typename_type<range_key<R>>,
        has_typename_type<range_mapped<R>>
      > {};

template<template<typename...> class C>
struct maybe_key_value_container
    : conjunction<
        bool_constant<(mandatory_template_arity<C>::value == 2)>,
        bool_constant<(template_arity<C>::value > 2)>
      > {};

template<
    template<typename...> class C,
    typename R,
    typename ArgSeq,
    bool =
        conjunction<
            has_typename_type<iter_val<R>>,
            has_typename_type<iter_key<R>>
        >::value
>
struct iter_key_val_category {
  using category = return_category<0>;
};
template<template<typename...> class C, typename R, typename... Args>
struct iter_key_val_category<C, R, type_sequence<Args...>, true> {
  using category =
      std::conditional_t<
          std::is_constructible<
              C<iter_key_t<R>, iter_val_t<R>, Args...>,
              ranges_to_input_iterator<R>&&, ranges_to_input_iterator<R>&&
          >::value,
          return_category<2, C<iter_key_t<R>, iter_val_t<R>, Args...>>,
          return_category<0>
      >;
};

template<template<typename...> class C, typename R, typename ArgSeq>
struct DEDUCE_EXPR_IMPL_KV;
template<template<typename...> class C, typename R, typename... Args>
struct DEDUCE_EXPR_IMPL_KV<C, R, type_sequence<Args...>> {
  using category =
      std::conditional_t<
          std::is_constructible<
              C<range_key_t<R>, range_mapped_t<R>, Args...>,
              R&&, Args&&...>::value,
          return_category<1, C<range_key_t<R>, range_mapped_t<R>, Args...>>,
      std::conditional_t<
          std::is_constructible<
              C<range_key_t<R>, range_mapped_t<R>, Args...>,
              from_range_t, R&&, Args&&...>::value,
          return_category<1, C<range_key_t<R>, range_mapped_t<R>, Args...>>,
      std::conditional_t<
          std::is_constructible<
              C<range_key_t<R>, range_mapped_t<R>, Args...>,
              ranges_to_input_iterator<R>&&, ranges_to_input_iterator<R>&&>::value,
          return_category<1, C<range_key_t<R>, range_mapped_t<R>, Args...>>,
          typename iter_key_val_category<C, R, type_sequence<Args...>>::category
      >>>;
};

template<template<typename...> class C, typename R, typename ArgSeq, bool = has_typename_type<iter_val<R>>::value>
struct iter_val_category {
  using category = return_category<0>;
};
template<template<typename...> class C, typename R, typename... Args>
struct iter_val_category<C, R, type_sequence<Args...>, true> {
  using category =
    std::conditional_t<
        std::is_constructible<
            C<iter_val_t<std::decay_t<R>>, Args...>,
            ranges_to_input_iterator<R>&&, ranges_to_input_iterator<R>&&
        >::value,
        return_category<2, C<iter_val_t<R>, Args...>>,
        return_category<0>>;
};

template<template<typename...> class C, typename R, typename ArgSeq>
struct DEDUCE_EXPR_IMPL_T;
template<template<typename...> class C, typename R, typename... Args>
struct DEDUCE_EXPR_IMPL_T<C, R, type_sequence<Args...>> {
  using category =
      std::conditional_t<
          std::is_constructible<
              C<range_value_t<R>, Args...>,
              R&&, Args&&... >::value,
          return_category<1, C<range_value_t<R>, Args...>>,
      std::conditional_t<
          std::is_constructible<
              C<range_value_t<R>, Args...>,
              from_range_t, R&&, Args&&... >::value,
          return_category<1, C<range_value_t<R>, Args...>>,
      std::conditional_t<
          std::is_constructible<
              C<range_value_t<R>, Args...>,
              ranges_to_input_iterator<R>&&, ranges_to_input_iterator<R>&& >::value,
          return_category<1, C<range_value_t<R>, Args...>>,
          typename iter_val_category<C, R, type_sequence<Args...>>::category
      >>>;
};

template<
    template<typename...> class C,
    typename R,
    typename ArgSeq,
    bool =
        conjunction<
            maybe_key_value_container<C>,
            is_key_value_range<R>
        >::value
>
struct DEDUCE_EXPR_IMPL;

template<template<typename...> class C, typename R, typename ArgSeq>
struct DEDUCE_EXPR_IMPL<C, R, ArgSeq, true> : DEDUCE_EXPR_IMPL_KV<C, R, ArgSeq> {};
template<template<typename...> class C, typename R, typename ArgSeq>
struct DEDUCE_EXPR_IMPL<C, R, ArgSeq, false> : DEDUCE_EXPR_IMPL_T<C, R, ArgSeq> {};

template<template<typename...> class C, typename R, typename... Args>
using DEDUCE_EXPR_CATEGORY = typename DEDUCE_EXPR_IMPL<C, R, type_sequence<Args...>>::category;

// #endif

} // namespace detail

/**
@addtogroup ranges
@{
@defgroup ranges_to__Range_conversions ranges::to
@brief constructs a new non-view object from an input range

The overloads of the range conversion function construct a new non-view object from a source range as its first
argument by calling a constructor taking a `range`, a `vccc::from_range_t` tagged ranged constructor,a constructor
taking an iterator-sentinel pair, or by back inserting each element of the source range into the
arguments-constructed object.

<H1>Example</H1>
@code{.cpp}
auto vec = vccc::views::iota(1, 5)
         | vccc::views::transform([](auto const v) { return v * 2; })
         | vccc::ranges::to<std::vector>();

auto lst = vec | vccc::views::take(3) | vccc::ranges::to<std::list<double>>();
@endcode

@sa [std::ranges::to](https://en.cppreference.com/w/cpp/ranges/to)
@}
*/

/// @addtogroup ranges_to__Range_conversions
/// @{

/// @brief Constructs an object of type `C` from the elements of `r`
template<typename C, typename R, typename... Args, std::enable_if_t<conjunction<
    ranges::input_range<R>,
    negation< ranges::view<C> >
>::value, int> = 0>
constexpr std::enable_if_t<(detail::ranges_to_1_tag<C, R, Args...>::value > 0), C>
to(R&& r, Args&&... args) {
  return detail::to_1<C>(detail::ranges_to_1_tag<C, R, Args...>{}, std::forward<R>(r), std::forward<Args>(args)...);
}

/// @brief Constructs an object of deduced type from the elements of `r`
template<template<typename...> class C, typename R, typename... Args, std::enable_if_t<
    ranges::input_range<R>::value, int> = 0>
constexpr typename detail::DEDUCE_EXPR_CATEGORY<C, R, Args...>::return_type
to(R&& r, Args&&... args) {
  using DEDUCE_EXPR = typename detail::DEDUCE_EXPR_CATEGORY<C, R, Args...>::return_type;
  return vccc::ranges::to<DEDUCE_EXPR>(std::forward<R>(r), std::forward<Args>(args)...);
}

namespace detail {

template<typename C, typename R, typename... Args>
constexpr C to_1(tag_5, R&& r, Args&&... args) {
  return vccc::ranges::to<C>(r | views::transform([](auto&& elem) {
      return to<range_value_t<C>>(std::forward<decltype(elem)>(elem));
  }), std::forward<Args>(args)...);
}

template<typename C, typename... Args>
class to_adaptor_closure : public range_adaptor_closure<to_adaptor_closure<C, Args...>> {
 public:
  template<typename... T>
  constexpr explicit to_adaptor_closure(in_place_t, T&&... arg)
      : tup_(std::forward<T>(arg)...) {}

  template<typename R, std::enable_if_t<input_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) const& {
    return call(std::forward<R>(r), std::index_sequence_for<Args...>{});
  }

  template<typename R, std::enable_if_t<input_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) && {
    return std::move(*this).call(std::forward<R>(r), std::index_sequence_for<Args...>{});
  }

 private:
  template<typename R, std::size_t... I>
  constexpr auto call(R&& r, std::index_sequence<I...>) const& {
    return vccc::ranges::to<C>(std::forward<R>(r), std::get<I>(tup_)...);
  }
  template<typename R, std::size_t... I>
  constexpr auto call(R&& r, std::index_sequence<I...>) && {
    return vccc::ranges::to<C>(std::forward<R>(r), std::get<I>(std::move(tup_))...);
  }

  std::tuple<Args...> tup_;
};

template<template<typename...> class>
struct template_type_holder;

template<template<typename...> class C, typename... Args>
class to_adaptor_closure<template_type_holder<C>, Args...>
    : public range_adaptor_closure<to_adaptor_closure<template_type_holder<C>, Args...>>
{
 public:
  template<typename... T>
  constexpr explicit to_adaptor_closure(in_place_t, T&&... arg)
      : tup_(std::forward<T>(arg)...) {}

  template<typename R, std::enable_if_t<input_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) const& {
    return call(std::forward<R>(r), std::index_sequence_for<Args...>{});
  }

  template<typename R, std::enable_if_t<input_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) && {
    return std::move(*this).call(std::forward<R>(r), std::index_sequence_for<Args...>{});
  }

 private:
  template<typename R, std::size_t... I>
  constexpr auto call(R&& r, std::index_sequence<I...>) const& {
    return vccc::ranges::to<C>(std::forward<R>(r), std::get<I>(tup_)...);
  }
  template<typename R, std::size_t... I>
  constexpr auto call(R&& r, std::index_sequence<I...>) && {
    return vccc::ranges::to<C>(std::forward<R>(r), std::get<I>(std::move(tup_))...);
  }

  std::tuple<Args...> tup_;
};

} // namespace detail

/// @brief Returns a perfect forwarding call wrapper that is also a
/// [RangeAdaptorClosureObject](https://en.cppreference.com/w/cpp/named_req/RangeAdaptorClosureObject)
template<typename C, typename... Args, std::enable_if_t<view<C>::value == false, int> = 0>
constexpr detail::to_adaptor_closure<C, std::decay_t<Args>...> to(Args&&... args) {
  return detail::to_adaptor_closure<C, std::decay_t<Args>...>(in_place, std::forward<Args>(args)...);
}

/// @brief Returns a perfect forwarding call wrapper with deduced return type that is also a
/// [RangeAdaptorClosureObject](https://en.cppreference.com/w/cpp/named_req/RangeAdaptorClosureObject)
template<template<typename...> class C, typename... Args>
constexpr detail::to_adaptor_closure<detail::template_type_holder<C>, std::decay_t<Args>...>
to(Args&&... args) {
  return detail::to_adaptor_closure<detail::template_type_holder<C>, std::decay_t<Args>...>(in_place, std::forward<Args>(args)...);
}

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_TO_HPP_