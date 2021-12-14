# /*
#  * Created by YongGyu Lee on 2021/12/15.
#  */
# 
# ifndef VCCC_TYPE_SUPPORT_DETAIL_TRAITS_H_
# define VCCC_TYPE_SUPPORT_DETAIL_TRAITS_H_
# 
# include <tuple>
# include <type_traits>
#
# include "vccc/type_support/core.hpp"

namespace vccc {
namespace detail {

template<typename T, typename = void>
struct has_value_type : std::false_type {};

template<typename T>
struct has_value_type<T, void_t<typename T::value_type>> : std::true_type {};

template<typename From, typename To> struct copy_reference             { using type = To;                              };
template<typename From, typename To> struct copy_reference<From&, To>  { using type = std::add_lvalue_reference_t<To>; };
template<typename From, typename To> struct copy_reference<From&&, To> { using type = std::add_rvalue_reference_t<To>; };

template<typename From, typename To> struct copy_cv                           { using type = To;                      };
template<typename From, typename To> struct copy_cv<const From, To>           { using type = std::add_const_t<To>;    };
template<typename From, typename To> struct copy_cv<volatile From, To>        { using type = std::add_volatile_t<To>; };
template<typename From, typename To> struct copy_cv<const volatile From, To>  { using type = std::add_cv_t<To>;       };

template<typename From, typename To> using copy_cv_t    = typename copy_cv<From, To>::type;
template<typename From, typename To> using copy_ref_t   = typename copy_reference<From, To>::type;
template<typename From, typename To> using copy_cvref_t = copy_cv_t<copy_ref_t<From, To>, To>;

template<typename T, typename = void>
struct is_tuple_like : std::false_type {};

template<typename T>
struct is_tuple_like<T, void_t<decltype(std::tuple_size<T>::value)>> : std::true_type {};

template<typename From, typename To,
         size_t v1 = std::tuple_size<std::decay_t<From>>::value,
         size_t v2 = std::tuple_size<std::decay_t<To>>::value,
         typename Indices1 = std::make_index_sequence<static_min<size_t, v1, v2>::value>,
         typename Indices2 = std::make_index_sequence<(v1 > v2 ? 0 : v2 - v1)>>
struct is_tuple_element_convertible;

template<typename From, typename To, size_t v1, size_t v2, size_t... I1, size_t... I2>
struct is_tuple_element_convertible<From, To, v1, v2, std::index_sequence<I1...>, std::index_sequence<I2...>>
    : conjunction<
        std::is_convertible<copy_cvref_t<From, std::tuple_element_t<I1, std::decay_t<From>>>, std::tuple_element_t<I1, std::decay_t<To>>>...,
        std::is_default_constructible<std::tuple_element_t<v1 + I2, To>>...
    > {
  static constexpr size_t converting_size = (v1 > v2) ? v2 : v1; 
};

template<typename Tuple, typename Range, bool v = has_value_type<Range>::value, typename Indices = std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>>
struct is_tuple_to_range_convertible : std::false_type {};

template<typename Tuple, typename Range, size_t... I>
struct is_tuple_to_range_convertible<Tuple, Range, true, std::index_sequence<I...>>
    : conjunction<std::is_convertible<copy_cvref_t<Tuple, std::tuple_element_t<I, std::decay_t<Tuple>>>, typename Range::value_type>...> {
  static constexpr size_t converting_size = std::tuple_size<std::decay_t<Tuple>>::value;
};

template<typename Range, typename Tuple, bool v = has_value_type<Range>::value, typename Indices = std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>>
struct is_range_to_tuple_convertible : std::false_type {};

template<typename Range, typename Tuple, size_t... I>
struct is_range_to_tuple_convertible<Range, Tuple, true, std::index_sequence<I...>>
    : conjunction<std::is_convertible<copy_cvref_t<Range, typename Range::value_type>, std::tuple_element_t<I, std::decay_t<Tuple>>>...> {
  static constexpr size_t converting_size = std::tuple_size<std::decay_t<Tuple>>::value;
};

template<typename From, typename To, bool v1 = is_tuple_like<std::decay_t<From>>::value, bool v2 = is_tuple_like<std::decay_t<To>>::value>
struct is_tuple_convertible;

template<typename From, typename To>
struct is_tuple_convertible<From, To, true, true> : is_tuple_element_convertible<From, To> {};

template<typename From, typename To>
struct is_tuple_convertible<From, To, true, false> : is_tuple_to_range_convertible<From, To> {};

template<typename From, typename To>
struct is_tuple_convertible<From, To, false, true> : is_range_to_tuple_convertible<From, To> {};

template<typename From, typename To>
struct is_tuple_convertible<From, To, false, false> : std::false_type {};

template<typename From, typename To>
using is_tuple_convertible_t = typename is_tuple_convertible<From, To>::type;

template<typename From, typename To>
struct is_convertible_wrapper : std::is_convertible<From, To> {
  static constexpr size_t converting_size = 0;
};

template<typename From, typename To>
using check_convert_to = disjunction<
    std::is_convertible<From, To>,
    is_tuple_convertible<From, To>
>;

} // namespace detail
} // namespace vccc

#endif // VCCC_TYPE_SUPPORT_DETAIL_TRAITS_H_
