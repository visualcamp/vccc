# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_NUMERIC_HPP
# define VCCC_TYPE_TRAITS_NUMERIC_HPP
#
# include <type_traits>

namespace vccc{


/** multiples_of */

template<typename T, T A, T B>
struct multiples_of : std::integral_constant<bool, A%B==0> {};

template<typename T, T A, T B>
using multiples_of_t = typename multiples_of<T, A, B>::type;

template<typename T, T A, T B>
constexpr bool multiples_of_v = multiples_of<T, A, B>::value;


/** is_odd */

template<typename T, T A>
struct is_odd : std::integral_constant<bool, A&1> {
  static_assert(A >= 0, "value must be natural number");
};

template<typename T, T A>
using is_odd_t = typename is_odd<T, A>::type;

template<typename T, T A>
constexpr bool is_odd_v = is_odd<T, A>::value;


/** is_even */

template<typename T, T A>
struct is_even : std::integral_constant<bool, (A&1) != 1> {
  static_assert(A>=0, "value must be natural number");
};

template<typename T, T A>
using is_even_t = typename is_even<T, A>::type;

template<typename T, T A>
constexpr bool is_even_v = is_even<T, A>::value;



}

# endif //VCCC_TYPE_TRAITS_NUMERIC_HPP
