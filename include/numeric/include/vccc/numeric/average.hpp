# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_NUMERIC_AVERAGE_HPP
# define VCCC_NUMERIC_AVERAGE_HPP
#
# include <cassert>
# include <iterator>
#
# include "vccc/type_traits.hpp"
# include "vccc/numeric/sum.hpp"

namespace vccc {

/// @addtogroup numeric
/// @{
///     @defgroup average_overloads average
///     @brief calculate average
/// @{

/**
@brief get average of iterator values [first, last)
@param first     begin of input iterator
@param last      end of input iterator
 */
template<typename InputIterator, std::enable_if_t<is_iterable<InputIterator>::value, int> = 0>
constexpr auto average(InputIterator first, InputIterator last) {
  assert(((void)"input size muse be larger than 0", first != last));
  using return_type = decltype(*first);
  if(first == last) return impl::default_value<decltype(*first)>() / static_cast<decay_if_float_t<return_type>>(1);
  return sum(first, last) / static_cast<decay_if_float_t<return_type>>(std::distance(first, last));
}

/**
@brief get average of iterator values [first, last)
@param first        begin of input iterator
@param last         end of input iterator
@param unary_op     custom unary operator( sum+=op(x) ... )
 */
template<typename InputIterator, typename UnaryOperation,
    std::enable_if_t<is_iterable<InputIterator>::value, int> = 0>
constexpr auto
average(InputIterator first, InputIterator last, UnaryOperation unary_op)
{
  assert(((void)"input size muse be larger than 0", first != last));
  using return_type = decltype(unary_op(*first));
  if(first == last) return static_cast<decay_if_float_t<return_type>>(sum(first, last, std::move(unary_op)));
  return sum(first, last, unary_op) / static_cast<decay_if_float_t<return_type>>(std::distance(first, last));
}

/**
@brief get average value of arithmetic types

Note:
- average of ints    -> return double
- average of floats  -> return float
- average of doubles -> return double
- average of long doubles -> return long double
@param ...numbers numbers
*/
template<typename ...Numbers,
         std::enable_if_t<conjunction<std::is_arithmetic<Numbers>...>::value, int> = 0>
constexpr inline auto
average(Numbers... numbers)
{
  return sum(numbers...) / static_cast<decay_if_float_t<Numbers...>>(sizeof...(numbers));
}


/**
@brief get average value of integers(result is floored)

@param ...ints numbers
*/
template<typename ...Ints,
         std::enable_if_t<conjunction<std::is_integral<Ints>...>::value, int> = 0>
constexpr inline
auto
int_average(Ints... ints)
{
  return sum(ints...) / static_cast<signed_bigger_type_t<Ints...>>(sizeof...(Ints));
}

/**
@brief get average value of custom types

@param ...args arguments
@return
*/
template<typename ...Args,
         std::enable_if_t<
           conjunction<
             negation<disjunction<is_iterable<Args>...>>,
             negation<disjunction<std::is_arithmetic<Args>...>>
           >::value, int> = 0>
constexpr inline
auto
average(const Args&... args)
{
  return sum(args...) / static_cast<double>(sizeof...(args));
}

/// @} average_overloads
/// @} numeric_average

} // namespace vccc

# endif // VCCC_NUMERIC_AVERAGE_HPP
