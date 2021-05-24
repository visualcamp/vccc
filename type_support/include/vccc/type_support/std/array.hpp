# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_STD_ARRAY_HPP
# define VCCC_TYPE_SUPPORT_STD_ARRAY_HPP
#
# include <array>

namespace vccc{

/**
@addtogroup type_support_at
@{

@defgroup type_support_at_array vccc::at (std::array)
Index-based value accessor
@addtogroup type_support_at_array
@{
*/
template<std::size_t i, typename T, int n>
constexpr inline
decltype(auto)
at(std::array<T, n>& arr) {
  static_assert(i < n, "Index out of bounds in vccc::at<> (std::array)");
  return arr[i];
}

template<std::size_t i, typename T, int n>
constexpr inline
decltype(auto)
at(const std::array<T, n>& arr) {
  static_assert(i < n, "Index out of bounds in vccc::at<> (const std::array)");
  return arr[i];
}

template<std::size_t i, typename T, int n>
constexpr inline
T&&
at(std::array<T, n>&& arr) {
  static_assert(i < n, "Index out of bounds in vccc::at<> (std::array&&)");
  return std::move(arr[i]);
}

template<std::size_t i, typename T, int n>
constexpr inline
const T&&
at(const std::array<T, n>&& arr) {
  static_assert(i < n, "Index out of bounds in vccc::at<> (const std::array&&)");
  return std::move(arr[i]);
}

//! @} type_support_at_array
//! @} type_support_at

}

# endif //VCCC_TYPE_SUPPORT_STD_ARRAY_HPP
