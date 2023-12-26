//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_TYPE_TRAITS_COMMON_REFERENCE_HPP_
#define VCCC_TYPE_TRAITS_COMMON_REFERENCE_HPP_

#include <type_traits>

#include "vccc/type_traits/detail/test_ternary.hpp"
#include "vccc/type_traits/copy_cvref.hpp"
#include "vccc/type_traits/has_typename_type.hpp"
#include "vccc/type_traits/remove_cvref.hpp"
#include "vccc/type_traits/simple_common_reference.hpp"

namespace vccc {

/// @addtogroup type_traits
/// @{
/// @addtogroup type_traits_common_reference__class__Miscellaneous_transformations common_reference
/// @brief determines the common reference type of a group of types
///
/// Determines the common reference type of the types `T...`, that is, the type to which all the types in `T...`
/// can be converted or bound. If such a type exists (as determined according to the rules below), the member type
/// names that type. Otherwise, there is no member type. The behavior is undefined if any of the types in `T...`
/// is an incomplete type other than (possibly cv-qualified) `void`.
/// @{

template<typename...>
struct common_reference;

template<typename... T>
using common_reference_t = typename common_reference<T...>::type;

template<typename T, typename U, template<typename> class TQual, template<typename> class UQual>
struct basic_common_reference {};

/// @}
/// @} type_traits

namespace impl {

template<typename T>
struct basic_common_reference_qual_gen {
  template<typename U>
  struct qual {
    using type = copy_cvref_t<T, U>;
  };
};

template<typename T1, typename T2>
struct common_reference_tag_1 : has_typename_type<simple_common_reference<T1, T2>> {};

template<typename T1, typename T2>
struct common_reference_tag_2
    : has_typename_type<
        basic_common_reference<
          remove_cvref_t<T1>,
          remove_cvref_t<T2>,
          basic_common_reference_qual_gen<T1>::template qual,
          basic_common_reference_qual_gen<T2>::template qual
        >
      > {};

template<typename T> T common_reference_val();

template<typename T1, typename T2, typename = void>
struct common_reference_tag_3 : std::false_type {};

template<typename T1, typename T2>
struct common_reference_tag_3<
    T1,
    T2,
    void_t<decltype( false ? common_reference_val<T1>() : common_reference_val<T2>() )>
    > : std::true_type {};

template<typename T1, typename T2, typename = void>
struct common_reference_tag_4 : std::false_type {};

template<typename T1, typename T2>
struct common_reference_tag_4<T1, T2, void_t<std::common_type_t<T1, T2>>> : std::true_type {};

template<typename T1, typename T2>
struct common_reference_tag
    : std::conditional_t<
        common_reference_tag_1<T1, T2>::value, std::integral_constant<int, 1>,
      std::conditional_t<
        common_reference_tag_2<T1, T2>::value, std::integral_constant<int, 2>,
      std::conditional_t<
        common_reference_tag_3<T1, T2>::value, std::integral_constant<int, 3>,
      std::conditional_t<
        common_reference_tag_4<T1, T2>::value, std::integral_constant<int, 4>,
        std::integral_constant<int, 0>
      >>>> {};


template<typename T1, typename T2, int = common_reference_tag<T1, T2>::value>
struct common_reference_two;

template<typename T1, typename T2>
struct common_reference_two<T1, T2, 0> {}; // no common reference type

template<typename T1, typename T2>
struct common_reference_two<T1, T2, 1> {
  using type = typename simple_common_reference<T1, T2>::type;
};

template<typename T1, typename T2>
struct common_reference_two<T1, T2, 2> {
  using type = typename basic_common_reference<
      remove_cvref_t<T1>,
      remove_cvref_t<T2>,
      basic_common_reference_qual_gen<T1>::template qual,
      basic_common_reference_qual_gen<T2>::template qual>::type;
};

template<typename T1, typename T2>
struct common_reference_two<T1, T2, 3> {
  using type = decltype( false ? common_reference_val<T1>() : common_reference_val<T2>() );
};

template<typename T1, typename T2>
struct common_reference_two<T1, T2, 4> {
  using type = std::common_type_t<T1, T2>;
};

template<bool /*true*/, typename T1, typename T2, typename... R>
struct common_reference_three : common_reference<common_reference_t<T1, T2>, R...> {};

template<typename T1, typename T2, typename... R>
struct common_reference_three<false, T1, T2, R...> {};

} // namespace impl

template<>
struct common_reference<> {};

template<typename T>
struct common_reference<T> {
  using type = T;
};

template<typename T1, typename T2>
struct common_reference<T1, T2> : impl::common_reference_two<T1, T2> {};

template<typename T1, typename T2, typename... R>
struct common_reference<T1, T2, R...>
    : impl::common_reference_three<
        has_typename_type<common_reference<T1, T2>>::value,
        T1,
        T2,
        R...
      > {};

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_COMMON_REFERENCE_HPP_
