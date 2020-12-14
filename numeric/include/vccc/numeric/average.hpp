# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_NUMERIC_AVERAGE_HPP
# define VCCC_NUMERIC_AVERAGE_HPP
#
# include <iterator>
# include "vccc/type_traits.hpp"
# include "vccc/numeric/sum.hpp"

namespace vccc{

/**
 * average of [first, last)
 */
template<typename InputIterator,
    VCCC_REQUIRE(iterable<InputIterator>)>
constexpr auto average(InputIterator first, InputIterator last){
  using ret_t = typename InputIterator::value_type;
  return sum(first, last) / static_cast<decay_if_float_t<typename InputIterator::value_type>>(std::distance(first, last));
}

template<typename InputIterator, typename UnaryOperation,
    VCCC_REQUIRE(iterable<InputIterator>)>
constexpr auto average(InputIterator first, InputIterator last, UnaryOperation unary_op){
  using ret_t = typename InputIterator::value_type;
  return sum(first, last, unary_op) / static_cast<decay_if_float_t<typename InputIterator::value_type>>(std::distance(first, last));
}

/**
 * average of arithmetic numbers
 *
 * average of ints    -> return double
 * average of floats  -> return float
 * average of doubles -> return double
 * average of long doubles -> return long double
 */
template<typename ...Numbers,
    VCCC_REQUIRE(are_arithmetic_v<Numbers...>)>
constexpr auto average(Numbers... numbers){
  return sum(numbers...) / static_cast<decay_if_float_t<Numbers...>>(sizeof...(numbers));
}

template<typename ...Ints,
    VCCC_REQUIRE(are_integral_v<Ints...>)>
constexpr auto int_average(Ints... ints){
  return sum(ints...) / static_cast<signed_bigger_type_t<Ints...>>(sizeof...(Ints));
}

/**
 * average of custom types
 */
template<typename ...Args,
    VCCC_REQUIRE(!iterable<Args...> && !are_arithmetic_v<Args...>)>
constexpr auto average(const Args&... args){
  return sum(args...) / static_cast<double>(sizeof...(args));
}

}

# endif //VCCC_NUMERIC_AVERAGE_HPP
