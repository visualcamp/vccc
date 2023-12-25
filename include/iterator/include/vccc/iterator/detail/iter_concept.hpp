//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_ITERATOR_DETAIL_ITER_CONCEPT_HPP_
#define VCCC_ITERATOR_DETAIL_ITER_CONCEPT_HPP_

#include <iterator>

#include "vccc/iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/iterator/iterator_tag.hpp"

namespace vccc {
namespace detail {

template<typename I> struct ITER_TRAITS_T { using type = cxx20_iterator_traits<I>; };
template<typename I> struct ITER_TRAITS_T<std::iterator_traits<I>> { using type = I; };
template<typename I> struct ITER_TRAITS_T<cxx20_iterator_traits<I>> { using type = I; };

template<typename I>
using ITER_TRAITS = typename ITER_TRAITS_T<I>::type;


template<typename I>
struct ITER_CONCEPT_IMPL_3 {};
template<typename I>
struct ITER_CONCEPT_IMPL_3<std::iterator_traits<I>> {
  using type = random_access_iterator_tag;
};
template<typename I>
struct ITER_CONCEPT_IMPL_3<cxx20_iterator_traits<I>> {
  using type = random_access_iterator_tag;
};

template<typename I, bool = has_typename_iterator_category<ITER_TRAITS<I>>::value /* false */>
struct ITER_CONCEPT_IMPL_2 : ITER_CONCEPT_IMPL_3<I> {};
template<typename I>
struct ITER_CONCEPT_IMPL_2<I, true> {
  using type = typename ITER_TRAITS<I>::iterator_category;
};

template<typename I, bool = is_specialized_iterator_traits<ITER_TRAITS<I>>::value /* false */>
struct ITER_CONCEPT_IMPL_1 : ITER_CONCEPT_IMPL_2<I> {};
template<typename I>
struct ITER_CONCEPT_IMPL_1<I, true> {
  using type = typename ITER_TRAITS<I>::iterator_concept;
};

template<typename I>
struct ITER_CONCEPT_T : ITER_CONCEPT_IMPL_1<I> {};

template<typename I>
using ITER_CONCEPT = typename ITER_CONCEPT_T<I>::type;

} // namespace detail
} // namespace vccc

#endif // VCCC_ITERATOR_DETAIL_ITER_CONCEPT_HPP_
