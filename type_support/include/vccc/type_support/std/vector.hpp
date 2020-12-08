# /*
#   Created by YongGyu Lee on 2020/12/08.
# */
#
# ifndef VCCC_TYPE_SUPPORT_STD_VECTOR_HPP
# define VCCC_TYPE_SUPPORT_STD_VECTOR_HPP
#
# include <vector>

namespace vc{

template<typename ...T>
std::vector<T...> reserved_vector(typename std::vector<T...>::size_type size){
  std::vector<T...> vec;
  vec.reserve(size);
  return vec;
}

}

#endif //VCCC_TYPE_SUPPORT_STD_VECTOR_HPP
