# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_STD_VECTOR_HPP
# define VCCC_TYPE_SUPPORT_STD_VECTOR_HPP
#
# include <algorithm>
# include <memory>
# include <vector>

namespace vccc {

//! @addtogroup type_support
//! @{

/**
 * @brief returns reserved vector
 *
 * @tparam T           vector element type
 * @tparam Allocator   vector element allocator(non is specialized)
 * @param size         reserving size
 * @return             reserved vector
 */

template<typename T, typename Allocator = std::allocator<T>>
std::vector<T, Allocator> reserved_vector(typename std::vector<T, Allocator>::size_type size){
  std::vector<T, Allocator> vec;
  vec.reserve(size);
  return vec;
}

/**
 * @addtogroup type_support_concat__func concat(std.vector)
 * @brief concatenate two vector
 * @{
 *
 * @brief concat two vectors
 *
 * @param to       target vector
 * @param from     source vector
 * @return
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

/// @}
//! @} type_support

} // namespace vccc

# endif // VCCC_TYPE_SUPPORT_STD_VECTOR_HPP
