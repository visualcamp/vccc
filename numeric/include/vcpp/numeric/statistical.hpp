//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCPP_STATISTICAL_HPP
#define VCPP_STATISTICAL_HPP

#include <cmath>
#include "vcpp/type_traits.hpp"

namespace vc {


/**
 * average of [first, last)
 */
template<typename InputIterator,
    VCPP_REQUIRE(vc::iterable<InputIterator>)>
constexpr auto average(InputIterator first, InputIterator last){
  using ret_t = typename InputIterator::value_type;
  return sum(first, last) / static_cast<decay_if_float_t<typename InputIterator::value_type>>(std::distance(first, last));
}

template<typename InputIterator, typename UnaryOperation,
    VCPP_REQUIRE(vc::iterable<InputIterator>)>
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
    VCPP_REQUIRE(vc::are_arithmetic_v<Numbers...>)>
constexpr auto average(Numbers... numbers){
  return sum(numbers...) / static_cast<decay_if_float_t<Numbers...>>(sizeof...(numbers));
}

template<typename ...Ints,
    VCPP_REQUIRE(are_integral_v<Ints...>)>
constexpr auto int_average(Ints... ints){
  return sum(ints...) / static_cast<signed_bigger_type_t<Ints...>>(sizeof...(Ints));
}

/**
 * average of custom types
 */
template<typename ...Args,
    VCPP_REQUIRE(!vc::iterable<Args...> && !vc::are_arithmetic_v<Args...>)>
constexpr auto average(const Args&... args){
  return sum(args...) / static_cast<double>(sizeof...(args));
}

/**
 * standard deviation
 */

template<typename InputIterator, typename T,
    VCPP_REQUIRE(iterable<InputIterator>)>
auto stddev(InputIterator first, InputIterator last, T avg){
  return std::sqrt(
      sum(first, last, [avg](const auto& next){return vc::square(next - avg);})
          / static_cast<decay_if_float_t<T>>(std::distance(first, last)) // TODO: 원래 1 빼야하는데 VectorMapping 에서 안빼길래 일단 안뺌
  );
}

template<typename InputIterator, VCPP_REQUIRE(iterable<InputIterator>)>
auto stddev(InputIterator first, InputIterator last){
  return stddev(first, last, average(first, last));
}

template<typename ...Numbers, VCPP_REQUIRE(!iterable<Numbers...>)>
auto stddev(Numbers... numbers){
  auto avg = average(numbers...);
  return std::sqrt(
      sum_custom([avg](auto next){return square(next - avg); }, numbers...)
          / static_cast<decay_if_float_t<Numbers...>>(sizeof...(Numbers))
  );
}

template<typename InputIterator>
constexpr auto avg_stddev(InputIterator first, InputIterator last){
  auto avg = average(first, last);
  return std::make_pair(avg, stddev(first, last, avg));
}

}

#endif //VCPP_STATISTICAL_HPP
