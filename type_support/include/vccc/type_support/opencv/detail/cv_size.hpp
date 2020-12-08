# /*
#   Created by YongGyu Lee on 2020/12/08.
# */
#
# ifndef VCCC_TYPE_SUPPORT_DETAIL_CV_SIZE_HPP
# define VCCC_TYPE_SUPPORT_DETAIL_CV_SIZE_HPP
#
# include <cstdlib>

namespace vc{namespace detail{

template<int n>
struct cv_size_n {
  static constexpr std::size_t value = n;
};

template<typename ...>
struct cv_size : cv_size_n<0> {};

template<typename T>
constexpr std::size_t cv_size_v = cv_size<T>::value;


template<typename T>
struct is_cv_type : std::integral_constant<bool, (cv_size<T>::value > 0) > {};

template<typename T>
constexpr auto is_cv_type_v = is_cv_type<T>::value;

#if __cplusplus >= 201402L
template<typename T1, typename T2>
constexpr std::size_t max_cv_size_v = std::max(cv_size_v<T1>, cv_size_v<T2>);

template<typename T1, typename T2>
constexpr std::size_t min_cv_size_v = std::min(cv_size_v<T1>, cv_size_v<T2>);
#else
template<typename T1, typename T2>
constexpr std::size_t max_cv_size_v = cv_size_v<T1> > cv_size_v<T2> ? cv_size_v<T1> : cv_size_v<T2>;

template<typename T1, typename T2>
constexpr std::size_t min_cv_size_v = cv_size_v<T1> > cv_size_v<T2> ? cv_size_v<T2> : cv_size_v<T1>;
#endif

template<typename T1, typename T2>
constexpr std::size_t diff_cv_size_v = max_cv_size_v<T1, T2> - min_cv_size_v<T1, T2>;

}}

#endif //VCCC_TYPE_SUPPORT_DETAIL_CV_SIZE_HPP
