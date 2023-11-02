# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_RANDOM_SAMPLE_HPP
# define VCCC_RANDOM_SAMPLE_HPP
#
# include <iterator>
# include <random>
# include <vector>
#
# include "vccc/random/pick.hpp"

namespace vccc {

//! @addtogroup random
//! @{

/**
@brief get shuffled random sample in [a, b], size k

Time complexity is \f$O(n)\f$

@tparam T           type
@tparam Container   return type
@param a            min value
@param b            max value
@param k            sample size
@param gen          random generator
@return             container that has samples
 */

template<typename T, typename Container = std::vector<T>>
Container
random_sample(T a, T b, T k, std::mt19937&& gen = std::mt19937(std::random_device{}()))
{
  auto elems = pick_k(a, b, k, gen);
  Container result(std::make_move_iterator(elems.begin()), std::make_move_iterator(elems.end()));
  std::shuffle(result.begin(), result.end(), gen);
  return result;
}

template<typename T, typename Container = std::vector<T>, typename Generator>
Container
random_sample(T a, T b, T k, Generator&& gen)
{
  auto elems = pick_k(a, b, k, gen);
  Container result(std::make_move_iterator(elems.begin()), std::make_move_iterator(elems.end()));
  std::shuffle(result.begin(), result.end(), gen);
  return result;
}

//! @}

} // namespace vccc

# endif // VCCC_RANDOM_SAMPLE_HPP
