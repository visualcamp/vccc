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
@addtogroup numeric
@{
    @defgroup numeric_average average
    @brief calculate average
@}

@addtogroup numeric_average
@{
*/

/**
@brief get average of iterator values [first, last)
@param first     begin of input iterator
@param last      end of input iterator
@return average
 */
template<typename InputIterator,
         VCCC_ENABLE_IF(iterable<InputIterator>)>
constexpr
auto
average(InputIterator first, InputIterator last)
{
  using ret_t = typename InputIterator::value_type;
  return sum(first, last) / static_cast<decay_if_float_t<typename InputIterator::value_type>>(std::distance(first, last));
}

/**
@brief get average of iterator values [first, last)
@param first        begin of input iterator
@param last         end of input iterator
@param unary_op     custom unary operator( sum+=op(x) ... )
@return average
 */
template<typename InputIterator, typename UnaryOperation,
         VCCC_ENABLE_IF(iterable<InputIterator>)>
constexpr
auto
average(InputIterator first, InputIterator last, UnaryOperation unary_op)
{
  using ret_t = typename InputIterator::value_type;
  return sum(first, last, unary_op) / static_cast<decay_if_float_t<typename InputIterator::value_type>>(std::distance(first, last));
}

/**
@brief get average value of arithmetic types

Note:
- average of ints    -> return double
- average of floats  -> return float
- average of doubles -> return double
- average of long doubles -> return long double
@param ...numbers numbers
@return average
*/
template<typename ...Numbers,
         VCCC_ENABLE_IF(are_arithmetic_v<Numbers...>)>
constexpr
auto
average(Numbers... numbers)
{
  return sum(numbers...) / static_cast<decay_if_float_t<Numbers...>>(sizeof...(numbers));
}


/**
@brief get average value of integers(result is floored)

@param ...ints numbers
@return average
*/
template<typename ...Ints,
         VCCC_ENABLE_IF(are_integral_v<Ints...>)>
constexpr
auto
int_average(Ints... ints)
{
  return sum(ints...) / static_cast<signed_bigger_type_t<Ints...>>(sizeof...(Ints));
}

/**
@brief get average value of custom types

@param ...args arguments
@return average
*/
template<typename ...Args,
         VCCC_ENABLE_IF(!iterable<Args...> && !are_arithmetic_v<Args...>)>
constexpr
auto
average(const Args&... args)
{
  return sum(args...) / static_cast<double>(sizeof...(args));
}

//! @} numeric_average
//! @} numeric

}

# endif //VCCC_NUMERIC_AVERAGE_HPP
