# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_STD_VECTOR_HPP
# define VCCC_TYPE_SUPPORT_STD_VECTOR_HPP
#
# include <vector>
# include <algorithm>
#
# include "vccc/utility.hpp"

namespace vccc{

//! @addtogroup type_support_at
//! @{

//! @cond ignored
template<typename T>
std::vector<T> reserved_vector(typename std::vector<T>::size_type size){
  std::vector<T> vec(size);
  vec.resize(0);
  ENSURES(vec.capacity() == size, "Failed to ");
  return vec;
}
//! @endcond

/**
@brief returns reserved vector

@tparam T           vector element type
@tparam Allocator   vector element allocator(non is specialized)
@param size         reserving size
@return             reserved vector
 */

template<typename T, typename Allocator>
std::vector<T, Allocator> reserved_vector(typename std::vector<T, Allocator>::size_type size){
  std::vector<T, Allocator> vec;
  vec.reserve(size);
  return vec;
}

/**
@brief concat two vectors

@param to       target vector
@param from     source vector
@return
 */

template<typename T, typename Allocator>
std::vector<T, Allocator>& concat(std::vector<T, Allocator>& to, const std::vector<T, Allocator>& from) {
  to.reserve(to.size() + from.size());
  for(int i=0; i<from.size(); ++i)
    to.emplace_back(from[i]);
  return to;
}

/**
@brief concat two vectors

@param to       target vector
@param from     source vector
@return
 */
template<typename T, typename Allocator>
std::vector<T, Allocator>& concat(std::vector<T, Allocator>& to, std::vector<T, Allocator>&& from) {
  to.reserve(to.size() + from.size());
  for(int i=0; i<from.size(); ++i)
    to.emplace_back(std::move(from[i]));
  return to;
}

//! @} type_support_at

//
//template<typename T, typename Allocator>
//std::vector<T, Allocator> slice(const std::vector<T, Allocator>& vec, std::size_t first){
//  return std::vector<T, Allocator>(std::min(vec.end(), vec.begin() + first),
//                                   vec.end());
//}
//
//template<typename T, typename Allocator>
//std::vector<T, Allocator> slice(std::vector<T, Allocator>&& vec, std::size_t first){
//  return std::vector<T, Allocator>(std::make_move_iterator(std::min(vec.end(), vec.begin() + first)),
//                                   std::make_move_iterator(vec.end()));
//}
//
//template<typename T, typename Allocator>
//std::vector<T, Allocator> slice(const std::vector<T, Allocator>& vec, std::size_t first, std::size_t last){
//  return std::vector<T, Allocator>(std::min(vec.end(), vec.begin() + first),
//                                   std::min(vec.end(), vec.begin() + last));
//}
//
//template<typename T, typename Allocator>
//std::vector<T, Allocator> slice(std::vector<T, Allocator>&& vec, std::size_t first, std::size_t last){
//  return std::vector<T, Allocator>(std::make_move_iterator(std::min(vec.end(), vec.begin() + first)),
//                                   std::make_move_iterator(std::min(vec.end(), vec.begin() + last)));
//}

}

# endif //VCCC_TYPE_SUPPORT_STD_VECTOR_HPP
