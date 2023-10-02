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

/**
@addtogroup numeric
@{
    @defgroup numeric_sum sum
    @brief calculate sum
@}

@addtogroup numeric_sum
@{
*/

namespace impl{
template<typename Arg>
constexpr inline auto sumImpl(const Arg& arg){
  return arg;
}

template<typename Arg1, typename Arg2>
constexpr inline auto sumImpl(const Arg1& arg1, const Arg2& arg2){
  return arg1 + arg2;
}

template<typename Arg, typename ...Args>
constexpr auto sumImpl(const Arg& arg, const Args&... args){
  return arg + sumImpl(args...);
}

template<typename T, VCCC_ENABLE_IF(std::is_class<std::decay_t<T>>::value)>
constexpr inline auto default_value() {
  return std::decay_t<T>();
}

template<typename T, VCCC_ENABLE_IF(!std::is_class<std::decay_t<T>>::value)>
constexpr inline auto default_value() {
  return static_cast<std::decay_t<T>>(0);
}

}

/**
@brief sum of iterator [first, last)
@param first    beginning iterator
@param last     ending iterator
@return         sum
*/
template<typename InputIterator, VCCC_ENABLE_IF(iterable<InputIterator>)>
constexpr
auto
sum(InputIterator first, InputIterator last)
{
  if(first == last) return impl::default_value<decltype(*first)>();
  auto s = *first;
  ++first;
  for(; first != last; ++first)
    s += *first;
  return s;
}

/**
@brief sum of iterator [first, last) with custom unary operator
@param first    beginning iterator
@param last     ending iterator
@param unary_op unary operator
@return         sum
*/
template<typename InputIterator, typename UnaryOperation, VCCC_ENABLE_IF(iterable<InputIterator>)>
constexpr
auto
sum(InputIterator first, InputIterator last, UnaryOperation unary_op)
{
  if(first == last) return impl::default_value<decltype(unary_op(*first))>();
  auto s = unary_op(*first);
  ++first;
  for(; first != last; ++first)
    s += unary_op(*first);
  return s;
}

/**
@brief sum of variadic
@param first    beginning iterator
@param last     ending iterator
@param unary_op unary operator
@return         sum
*/
template<typename ...Args, VCCC_ENABLE_IF(!iterable<Args...>)>
constexpr inline
auto
sum(const Args&... args)
{
  return impl::sumImpl(args...);
}

/**
@brief sum of variadic with custom operator
@param unary_op unary operator
@return         sum
*/
template<typename UnaryOperation, typename Arg>
constexpr inline
auto
sum_custom(const UnaryOperation& unary_op, const Arg& arg)
{
  return unary_op(arg);
}

template<typename UnaryOperation, typename Arg, typename ...Args>
constexpr inline
auto
sum_custom(const UnaryOperation& unary_op, const Arg& arg, const Args&... args)
{
  return unary_op(arg) + sum_custom(unary_op, args...);
}


/**
@brief get square
@param val  value
@return     squared value
*/
template<typename T>
constexpr
auto
square(const T& val)
{
//  return std::pow(val, 2);
  return val*val;
}


/**
@brief square sum of iterator [first, last)
@param first    beginning iterator
@param last     ending iterator
@return         squared sum
*/
template<typename InputIterator, VCCC_ENABLE_IF(iterable<InputIterator>)>
constexpr
auto
square_sum(InputIterator first, InputIterator last)
{
  return sum(first, last, [](const auto& val){ return square(val); });
}

template<typename Arg>
constexpr
auto
square_sum(const Arg& arg)
{
  return square(arg);
}

template<typename Arg1, typename Arg2, VCCC_ENABLE_IF((!iterable<Arg1, Arg2>))>
constexpr
auto
square_sum(const Arg1& arg1, const Arg2& arg2)
{
  return square(arg1) + square(arg2);
}

/**
@brief square sum of variadic
@param arg, ...args     args
@return                 squared sum
*/
template<typename Arg, typename ...Args, VCCC_ENABLE_IF(!iterable<Arg>)>
constexpr
auto
square_sum(const Arg& arg, const Args&... args)
{
  return square(arg) + square_sum(args...);
}

//! @} numeric_sum

}

# endif //VCCC_NUMERIC_SUM_HPP
