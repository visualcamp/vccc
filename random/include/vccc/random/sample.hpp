# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_RANDOM_SAMPLE_HPP
# define VCCC_RANDOM_SAMPLE_HPP
#
# include <vector>
# include <iterator>
# include "vccc/random/pick.hpp"

namespace vccc{

template<typename T, typename Container = std::vector<T>>
Container random_sample(T a, T b, T k, std::mt19937&& gen = std::mt19937(std::random_device{}())){
  auto elems = pick_k(a, b, k, gen);
  Container result(std::make_move_iterator(elems.begin()), std::make_move_iterator(elems.end()));
  std::shuffle(result.begin(), result.end(), gen);
  return result;
}

template<typename T, typename Container = std::vector<T>, typename Generator>
Container random_sample(T a, T b, T k, Generator&& gen){
  auto elems = pick_k(a, b, k, gen);
  Container result(std::make_move_iterator(elems.begin()), std::make_move_iterator(elems.end()));
  std::shuffle(result.begin(), result.end(), gen);
  return result;
}

}

# endif //VCCC_RANDOM_SAMPLE_HPP
