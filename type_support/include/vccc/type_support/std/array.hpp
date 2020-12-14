# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_STD_ARRAY_HPP
# define VCCC_TYPE_SUPPORT_STD_ARRAY_HPP
#
# include <array>

namespace vccc{

template<std::size_t i, typename T, int n>
constexpr
decltype(auto)
at(std::array<T, n>& arr) {
  static_assert(i < n, "Index out of bounds in at<> (std::array)");
  return arr[i];
}

template<std::size_t i, typename T, int n>
constexpr
decltype(auto)
at(const std::array<T, n>& arr) {
  static_assert(i < n, "Index out of bounds in at<> (const std::array)");
  return arr[i];
}

template<std::size_t i, typename T, int n>
constexpr
T&&
at(std::array<T, n>&& arr) {
  static_assert(i < n, "Index out of bounds in at<> (std::array&&)");
  return std::move(arr[i]);
}

template<std::size_t i, typename T, int n>
constexpr
const T&&
at(const std::array<T, n>&& arr) {
  static_assert(i < n, "Index out of bounds in at<> (const std::array&&)");
  return std::move(arr[i]);
}

}

# endif //VCCC_TYPE_SUPPORT_STD_ARRAY_HPP
