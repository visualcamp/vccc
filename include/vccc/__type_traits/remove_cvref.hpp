# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_CVREF_HPP
# define VCCC_TYPE_TRAITS_CVREF_HPP
#
# include <type_traits>

namespace vccc {

/// @addtogroup type_traits
/// @{
/// @defgroup type_traits_remove_cvref__class__Miscellaneous_transformations remove_cvref
/// @brief combines std::remove_cv and std::remove_reference
/// @{

template<typename T>
struct remove_cvref {
  using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template<typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

/// @}
/// @} type_traits

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_CVREF_HPP
