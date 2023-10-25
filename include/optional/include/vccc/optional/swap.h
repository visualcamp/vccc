# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef VCCC_OPTIONAL_SWAP_H_
# define VCCC_OPTIONAL_SWAP_H_
#
# include <algorithm>
# include <type_traits>
# include <utility>
#
# include "vccc/optional/internal/is_swappable.h"
# include "vccc/optional/forward_declare.h"

namespace std {

//! @addtogroup optional
//! @{

/**
 * @brief specializes the std::swap algorithm
 */
template<typename T,
  ::std::enable_if_t<
    ::std::is_move_constructible<T>::value && ::vccc::internal::optional::is_swappable<T>::value,
  int> = 0>
void swap(::vccc::optional<T>& lhs, ::vccc::optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

//! @} optional

} // namespace std

# endif // VCCC_OPTIONAL_SWAP_H_
