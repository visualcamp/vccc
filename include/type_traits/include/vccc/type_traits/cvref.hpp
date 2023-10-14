# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_CVREF_HPP
# define VCCC_TYPE_TRAITS_CVREF_HPP
#
# include <type_traits>

namespace vccc {

/**
@addtogroup type_traits
@{
    @defgroup remove_cvref remove_cvref
    @brief combines std::remove_cv and std::remove_reference
@}

@addtogroup remove_cvref
@{
*/

template<typename T>
struct remove_cvref {
  using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template<typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

//! @} remove_cvref

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_CVREF_HPP
