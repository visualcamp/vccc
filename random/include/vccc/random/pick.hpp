# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_RANDOM_PICK_HPP
# define VCCC_RANDOM_PICK_HPP
#
# include <unordered_set>
# include <random>
# include <type_traits>

namespace vccc {

template<typename T, typename Container = std::unordered_set<T>, typename RandomGenerator>
Container pick_k(T a, T b, T k, RandomGenerator&& gen) {
  static_assert(std::is_integral<T>::value, "type must be integer (vccc::pick_k)");
  std::unordered_set<int> elems;
  elems.reserve(k);

  for (auto r = b - k + 1; r <= b; ++r) {
    auto v = std::uniform_int_distribution<>(a, r)(gen);

    if (!elems.emplace(v).second) {
      elems.emplace(r);
    }
  }
  return elems;
}

}

# endif //VCCC_RANDOM_INCLUDE_VCCC_RANDOM_DETAIL_PICK_HPP
