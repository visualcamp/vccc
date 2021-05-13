# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_NUMERIC_STDDEV_HPP
# define VCCC_NUMERIC_STDDEV_HPP
#
# include "vccc/numeric/average.hpp"

namespace vccc {

/**
@addtogroup numeric
@{
    @defgroup numeric_stddev standard deviation
    @brief calculate standard deviation

    \f$ {\large \sigma=\sqrt{\frac{\sum{(x_i - \mu)}}{N}}} \f$
@}

@addtogroup numeric_stddev
@{
*/

/**
@brief calculate standard deviation with pre-calculated average on iterators

@param first    beginning iterator
@param last     ending iterator
@param avg      pre-calculated average
@return standard deviation
 */
template<typename InputIterator, typename T,
         VCCC_ENABLE_IF(iterable < InputIterator >)>
auto
stddev(InputIterator first, InputIterator last, T avg)
{
  return std::sqrt(
      sum(first, last, [avg](const auto& next) { return square(next - avg); })
          / static_cast<decay_if_float_t<T>>(std::distance(first, last)) // TODO: subtract 1
  );
}

/**
@brief calculate standard deviation on iterators

@param first    beginning iterator
@param last     ending iterator
@return standard deviation
 */
template<typename InputIterator, VCCC_ENABLE_IF(iterable<InputIterator>)>
auto
stddev(InputIterator first, InputIterator last)
{
  return stddev(first, last, average(first, last));
}

/**
@brief calculate standard deviation on variadics

@param ...numbers   numbers
@return standard deviation
 */
template<typename ...Numbers, VCCC_ENABLE_IF(!iterable<Numbers...>)>
auto
stddev(Numbers... numbers)
{
  return std::sqrt(
      sum_custom([avg = average(numbers...)](auto next) { return square(next - avg); }, numbers...)
          / static_cast<decay_if_float_t < Numbers... >> (sizeof...(Numbers))
  );
}

/**
@brief calculate standard deviation and average

@param ...numbers   numbers
@return pair of average and standard deviation
 */
template<typename InputIterator>
auto
avg_stddev(InputIterator first, InputIterator last)
{
  auto avg = average(first, last);
  return std::make_pair(avg, stddev(first, last, avg));
}

//! @} numeric_stddev

}

# endif //VCCC_NUMERIC_STDDEV_HPP
