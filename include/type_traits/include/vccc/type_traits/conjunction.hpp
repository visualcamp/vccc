//
// Created by cosge on 2023-10-14.
//

#ifndef VCCC_TYPE_TRAITS_CONJUNCTION_HPP
#define VCCC_TYPE_TRAITS_CONJUNCTION_HPP

#include <type_traits>

namespace vccc {

//! @addtogroup type_traits
//! @{
//! @addtogroup type_traits_conjunction__Operation_on_traits conjunction
//! @{

/**
 * conjunction
 * @tparam ...B
 */
template<typename ...B>
struct conjunction;

//! @}
//! @} type_traits

template<>
struct conjunction<> : std::true_type {};

template<typename B1>
struct conjunction<B1> : B1 {};

template<typename B1, typename ...BN>
struct conjunction<B1, BN...> : std::conditional_t<bool(B1::value), conjunction<BN...>, B1> {};

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_CONJUNCTION_HPP
