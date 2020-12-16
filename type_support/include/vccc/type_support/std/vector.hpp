# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_STD_VECTOR_HPP
# define VCCC_TYPE_SUPPORT_STD_VECTOR_HPP
#
# include <vector>

namespace vccc{

template<typename ...T>
std::vector<T...> reserved_vector(typename std::vector<T...>::size_type size){
  std::vector<T...> vec;
  vec.reserve(size);
  return vec;
}

template<typename T, typename Allocator>
std::vector<T, Allocator>& concat(std::vector<T, Allocator>& to, const std::vector<T, Allocator>& from) {
  static_assert(std::is_copy_constructible<T>::value, "");
  to.reserve(to.size() + from.size());
  for(int i=0; i<from.size(); ++i)
    to.emplace_back(from[i]);
  return to;
}

template<typename T, typename Allocator>
std::vector<T, Allocator>& concat(std::vector<T, Allocator>& to, std::vector<T, Allocator>&& from) {
  static_assert(std::is_move_constructible<T>::value, "");
  to.reserve(to.size() + from.size());
  for(int i=0; i<from.size(); ++i)
    to.emplace_back(std::move(from[i]));
  return to;
}

}

# endif //VCCC_TYPE_SUPPORT_STD_VECTOR_HPP
