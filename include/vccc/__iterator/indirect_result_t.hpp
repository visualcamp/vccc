//
// Created by yonggyulee on 2023/12/29.
//

#ifndef VCCC_ITERATOR_INDIRECT_RESULT_T_HPP
#define VCCC_ITERATOR_INDIRECT_RESULT_T_HPP

#include "vccc/__concepts/invocable.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__iterator/indirectly_readable.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"

namespace vccc {
namespace detail {

template<bool /* true */, typename F, typename... Is>
struct indirect_result_impl_2 {
  using type = invoke_result_t<F, iter_reference_t<Is>...>;
};

template<typename F, typename... Is>
struct indirect_result_impl_2<false, F, Is...> {};

template<bool /* true */, typename F, typename... Is>
struct indirect_result_impl_1
    : indirect_result_impl_2<
          invocable<F, iter_reference_t<Is>...>::value,
          F,
          Is...
      > {};

template<typename F, typename... Is>
struct indirect_result_impl_1<false, F, Is...> {};

} // namespace detail

/// @addtogroup iterator
/// @{

template<typename F, typename... Is>
struct indirect_result
    : detail::indirect_result_impl_1<
          conjunction<
            indirectly_readable<Is>...,
            has_typename_type<iter_reference<Is>>...
          >::value,
          F,
          Is...
      > {};

template<typename F, typename... Is>
using indirect_result_t = typename indirect_result<F, Is...>::type;

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECT_RESULT_T_HPP
