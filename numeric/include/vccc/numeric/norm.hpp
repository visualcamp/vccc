# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_NUMERIC_NORM_HPP
# define VCCC_NUMERIC_NORM_HPP
#
# include "vccc/type_traits.hpp"
# include "vccc/numeric/sum.hpp"

namespace vccc{

template<typename InputIterator, VCCC_REQUIRE(iterable<InputIterator>)>
auto norm(InputIterator first, InputIterator last) {
  return std::sqrt(square_sum(first, last));
}

template<typename Arg, VCCC_REQUIRE(!iterable<Arg>)>
auto norm(Arg arg) {
  return std::abs(arg);
}

template<typename Arg, typename ...Args, VCCC_REQUIRE((!iterable<Arg, Args...>))>
auto norm(Arg arg, Args... args) {
  return std::sqrt(square_sum(arg, args...));
}

}

#endif //VCCC_NUMERIC_NORM_HPP
