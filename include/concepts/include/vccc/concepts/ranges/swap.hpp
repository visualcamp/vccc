//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_CONCEPTS_RANGES_SWAP_HPP_
#define VCCC_CONCEPTS_RANGES_SWAP_HPP_

#include <type_traits>

#include "vccc/type_traits/bool_constant.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/disjunction.hpp"
#include "vccc/type_traits/is_swappable.hpp"

namespace vccc {
namespace concepts {

template<typename T, typename U>
struct swappable;

} // namespace concepts

namespace ranges {

namespace swap_impl {
template <typename T>
void swap(T&, T&) = delete;

template<typename T, typename U>
struct swap_category_1
    : conjunction<
        disjunction<
          std::is_class<T>, std::is_enum<T>, std::is_union<T>,
          std::is_class<U>, std::is_enum<U>, std::is_union<U>>,
        is_swappable_with<T&, U&>
      > {};


//template<typename T, typename U>
//struct swap_category_2a
//    : bool_constant<noexcept((void)ranges::swap_ranges(t, u)) == noexcept(ranges::swap(*t, *u))> {};

template<typename T, typename U>
struct swap_category_2 : std::false_type {};

//template<typename T, typename U, std::size_t N>
//struct swap_category_2<T[N], U[N]>
//    : std::conditional_t<
//        concepts::swappable<decltype(*std::declval<T(&)[N]>()), decltype(*std::declval<U(&)[N]>())>::value,
//        swap_category_2a<T, U>,
//        std::false_type
//      > {};



template<typename T, typename U>
struct swap_category
    : std::conditional_t<
        swap_category_1<T, U>::value, std::integral_constant<int, 1>,
        int
      > {};

template<
    typename T,
    typename U,
    int = swap_category<T, U>::value>
struct swap_obj;

} // swap_impl

template<typename T, typename U>
constexpr void swap(T&& t, U&& u) {

}

} // namespace ranges
} // namespace vccc

#endif // VCCC_CONCEPTS_RANGES_SWAP_HPP_
