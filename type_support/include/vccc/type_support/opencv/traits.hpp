# /*
#  * Created by YongGyu Lee on 2021/03/17.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_TRAITS_HPP_
# define VCCC_TYPE_SUPPORT_OPENCV_TRAITS_HPP_
#
# include <type_traits>

namespace vccc {

/// cv_size
template<int n>
struct cv_size_n {
  static constexpr std::size_t value = n;
};

template<typename ...>
struct cv_size : cv_size_n<0> {};

template<typename T>
constexpr std::size_t cv_size_v = cv_size<T>::value;

/// is_cv_type

template<typename T>
struct is_cv_type : public std::integral_constant<bool, (cv_size<T>::value > 0) > {};

template<typename T>
using is_cv_type_t = typename is_cv_type<T>::type;

template<typename T>
constexpr bool is_cv_type_v = is_cv_type<T>::value;


template<typename T1, typename T2>
constexpr std::size_t max_cv_size_v = cv_size_v<T1> > cv_size_v<T2> ? cv_size_v<T1> : cv_size_v<T2>;

template<typename T1, typename T2>
constexpr std::size_t min_cv_size_v = cv_size_v<T1> > cv_size_v<T2> ? cv_size_v<T2> : cv_size_v<T1>;

template<typename T1, typename T2>
constexpr std::size_t diff_cv_size_v = max_cv_size_v<T1, T2> - min_cv_size_v<T1, T2>;

}

# endif //VCCC_TYPE_SUPPORT_OPENCV_TRAITS_HPP_
