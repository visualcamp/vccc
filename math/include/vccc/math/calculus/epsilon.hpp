# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_MATH_CALCULUS_EPSILON_HPP
# define VCCC_MATH_CALCULUS_EPSILON_HPP
#
# include <limits>
# include <cmath>

namespace vccc{

template<typename T>
auto epsilon(){
  static const auto e = std::sqrt(std::numeric_limits<T>::epsilon());
  return e;
}

}


# endif //VCCC_MATH_CALCULUS_EPSILON_HPP
