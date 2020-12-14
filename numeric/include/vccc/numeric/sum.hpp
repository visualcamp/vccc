# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_NUMERIC_SUM_HPP
# define VCCC_NUMERIC_SUM_HPP
#
# include "vccc/type_traits.hpp"
# include <numeric>

namespace vccc {

namespace impl{
template<typename Arg>
constexpr auto sumImpl(const Arg& arg){
  return arg;
}

template<typename Arg1, typename Arg2>
constexpr auto sumImpl(const Arg1& arg1, const Arg2& arg2){
  return arg1 + arg2;
}

template<typename Arg, typename ...Args>
constexpr auto sumImpl(const Arg& arg, const Args&... args){
  return arg + sumImpl(args...);
}
}

/**
 * sum of [first, last)
 */
template<typename InputIterator, VCCC_REQUIRE(iterable<InputIterator>)>
constexpr auto sum(InputIterator first, InputIterator last){
  auto s = *first;
  if (first == last) return s-s;
  ++first;
  for(; first != last; ++first)
    s += *first;
  return s;
}

template<typename InputIterator, typename UnaryOperation, VCCC_REQUIRE(iterable<InputIterator>)>
constexpr auto sum(InputIterator first, InputIterator last, UnaryOperation unary_op){
  auto s = unary_op(*first);
  if (first == last) return s-s;
  ++first;
  for(; first != last; ++first)
    s += unary_op(*first);
  return s;
}

/**
 * sum of args
 */
template<typename ...Args, VCCC_REQUIRE(!iterable<Args...>)>
constexpr auto sum(const Args&... args){
  return impl::sumImpl(args...);
}

/**
 *  custom unary_operation of parameter packs
 */
template<typename UnaryOperation, typename Arg>
constexpr auto sum_custom(const UnaryOperation& unary_op, const Arg& arg){
  return unary_op(arg);
}

template<typename UnaryOperation, typename Arg, typename ...Args>
constexpr auto sum_custom(const UnaryOperation& unary_op, const Arg& arg, const Args&... args){
  return unary_op(arg) + sum_custom(unary_op, args...);
}


/**
 * square
 */
template<typename T>
constexpr auto square(const T& val){
//  return std::pow(val, 2);
  return val*val;
}

/**
 * sum of squares
 */

template<typename InputIterator, VCCC_REQUIRE(iterable<InputIterator>)>
constexpr auto square_sum(InputIterator first, InputIterator last){
  return sum(first, last, [](const auto& val){ return square(val); });
}

template<typename Arg>
constexpr auto square_sum(const Arg& arg){
  return square(arg);
}

template<typename Arg1, typename Arg2, VCCC_REQUIRE((!iterable<Arg1, Arg2>))>
constexpr auto square_sum(const Arg1& arg1, const Arg2& arg2){
  return square(arg1) + square(arg2);
}

template<typename Arg, typename ...Args, VCCC_REQUIRE(!iterable<Arg>)>
constexpr auto square_sum(const Arg& arg, const Args&... args){
  return square(arg) + square_sum(args...);
}

}

# endif //VCCC_NUMERIC_SUM_HPP
