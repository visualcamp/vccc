//
// Created by yonggyulee on 1/28/24.
//

#ifndef VCCC_ALGORITHM_RANGES_DETAIL_INPUT_RANGE_CHECK_HPP_
#define VCCC_ALGORITHM_RANGES_DETAIL_INPUT_RANGE_CHECK_HPP_

#include "vccc/__iterator/forward_iterator.hpp"
#include "vccc/__iterator/indirect_binary_predicate.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<
    template<typename F, typename I, typename...> class Test,
    template<typename I> class IteratorModel,
    typename I, typename S, typename Proj, typename Pred,
    bool = conjunction<
        IteratorModel<I>,
        sentinel_for<S, I>,
        projectable<I, Proj>>::value /* true */
>
struct algo_check_unary_iterator : Test<Pred, projected<I, Proj>> {};

template<
    template<typename F, typename I, typename...> class Test,
    template<typename I> class IteratorModel,
    typename I, typename S, typename Proj, typename Pred
>
struct algo_check_unary_iterator<Test, IteratorModel, I, S, Proj, Pred, false> : std::false_type {};


template<
    template<typename F, typename I1, typename I2> class Test,
    template<typename I> class IteratorModel,
    typename I, typename S, typename T, typename Proj, typename Pred,
    bool = conjunction<
        IteratorModel<I>,
        sentinel_for<S, I>,
        projectable<I, Proj>>::value /* true */
>
struct algo_check_binary_iterator : Test<Pred, projected<I, Proj>, T> {};

template<
    template<typename F, typename I1, typename I2> class Test,
    template<typename I> class IteratorModel,
    typename I, typename S, typename T, typename Proj, typename Pred>
struct algo_check_binary_iterator<Test, IteratorModel, I, S, T, Proj, Pred, false> : std::false_type {};




template<
    template<typename F, typename I, typename...> class Test,
    typename I, typename Proj, typename Pred,
    bool = projectable<I, Proj>::value /* true */
>
struct algo_check_unary_range_2 : Test<Pred, projected<I, Proj>> {};

template<
    template<typename F, typename I, typename...> class Test,
    typename I, typename Proj, typename Pred
>
struct algo_check_unary_range_2<Test, I, Proj, Pred, false> : std::false_type {};

template<
    template<typename F, typename I, typename...> class Test,
    template<typename R> class RangeModel,
    typename R, typename Proj, typename Pred,
    bool = RangeModel<R>::value /* true */
>
struct algo_check_unary_range : algo_check_unary_range_2<Test, iterator_t<R>, Proj, Pred> {};

template<
    template<typename F, typename I, typename...> class Test,
    template<typename R> class RangeModel,
    typename R, typename Proj, typename Pred
>
struct algo_check_unary_range<Test, RangeModel, R, Proj, Pred, false> : std::false_type {};



template<
    template<typename F, typename I1, typename I2> class Test,
    typename I, typename T, typename Proj, typename Pred,
    bool = projectable<I, Proj>::value /* true */>
struct algo_check_binary_range_2 : Test<Pred, projected<I, Proj>, T> {};

template<
    template<typename F, typename I1, typename I2> class Test,
    typename I, typename T, typename Proj, typename Pred>
struct algo_check_binary_range_2<Test, I, T, Proj, Pred, false> : std::false_type {};

template<
    template<typename F, typename I1, typename I2> class Test,
    template<typename R> class RangeModel,
    typename R, typename T, typename Proj, typename Pred,
    bool = RangeModel<R>::value /* true */>
struct algo_check_binary_range : algo_check_binary_range_2<Test, iterator_t<R>, T, Proj, Pred> {};

template<
    template<typename F, typename I1, typename I2> class Test,
    template<typename R> class RangeModel,
    typename R, typename T, typename Proj, typename Pred>
struct algo_check_binary_range<Test, RangeModel, R, T, Proj, Pred, false> : std::false_type {};


// algo_check_unary_input_iterator
// : test if expression `Test<Pred, projected<I, Proj>>` is valid

template<template<typename F, typename I, typename...> class Test, typename I, typename S, typename Proj, typename Pred>
struct algo_check_unary_input_iterator : algo_check_unary_iterator<Test, input_iterator, I, S, Proj, Pred> {};


// algo_check_unary_forward_iterator
// : test if expression `Test<Pred, projected<I, Proj>>` is valid

template<
    template<typename F, typename I, typename...> class Test,
    typename I, typename S, typename Proj, typename Pred>
struct algo_check_unary_forward_iterator : algo_check_unary_iterator<Test, forward_iterator, I, S, Proj, Pred> {};


// algo_check_binary_input_iterator
// : test if expresssion `Test<Pred, projected<I, Proj>, T>` is valid

template<
    template<typename F, typename I1, typename I2> class Test,
    typename I, typename S, typename T, typename Proj, typename Pred>
struct algo_check_binary_input_iterator : algo_check_binary_iterator<Test, input_iterator, I, S, T, Proj, Pred> {};


// algo_check_unary_input_range
// : test if expression `Test<Pred, projected<iterator_t<R>, Proj>>` is valid

template<template<typename F, typename I, typename...> class Test, typename R, typename Proj, typename Pred>
struct algo_check_unary_input_range : algo_check_unary_range<Test, input_range, R, Proj, Pred> {};

// algo_check_unary_forward_range
// : test if expression `Test<Pred, projected<iterator_t<R>, Proj>>` is valid

template<template<typename F, typename I, typename...> class Test, typename R, typename Proj, typename Pred>
struct algo_check_unary_forward_range : algo_check_unary_range<Test, forward_range, R, Proj, Pred> {};

// algo_check_binary_input_range
// : test if expression `Test<Pred, projected<iterator_t<R>, Proj>, T>` is valid
template<
    template<typename F, typename I1, typename I2> class Test,
    typename R, typename T, typename Proj, typename Pred>
struct algo_check_binary_input_range : algo_check_binary_range<Test, input_range, R, T, Proj, Pred> {};

} // namespace vccc
} // namespace ranges
} // namespace detail

#endif // VCCC_ALGORITHM_RANGES_DETAIL_INPUT_RANGE_CHECK_HPP_
