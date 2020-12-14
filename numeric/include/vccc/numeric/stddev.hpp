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
 * standard deviation
 */

template<typename InputIterator, typename T,
VCCC_REQUIRE(iterable < InputIterator >)>
auto stddev(InputIterator first, InputIterator last, T avg) {
  return std::sqrt(
      sum(first, last, [avg](const auto& next) { return square(next - avg); })
          / static_cast<decay_if_float_t<T>>(std::distance(first, last)) // TODO: 원래 1 빼야하는데 VectorMapping 에서 안빼길래 일단 안뺌
  );
}

template<typename InputIterator, VCCC_REQUIRE(iterable<InputIterator>)>
auto stddev(InputIterator first, InputIterator last) {
  return stddev(first, last, average(first, last));
}

template<typename ...Numbers, VCCC_REQUIRE(!iterable<Numbers...>)>
auto stddev(Numbers... numbers) {
  return std::sqrt(
      sum_custom([avg = average(numbers...)](auto next) { return square(next - avg); }, numbers...)
          / static_cast<decay_if_float_t < Numbers... >> (sizeof...(Numbers))
  );
}

template<typename InputIterator>
constexpr auto avg_stddev(InputIterator first, InputIterator last) {
  auto avg = average(first, last);
  return std::make_pair(avg, stddev(first, last, avg));
}

}

# endif //VCCC_NUMERIC_STDDEV_HPP
