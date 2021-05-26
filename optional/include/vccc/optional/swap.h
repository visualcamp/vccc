# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef VCCC_OPTIONAL_SWAP_H_
# define VCCC_OPTIONAL_SWAP_H_
#
# include <type_traits>
#
# include "vccc/optional/forward_declare.h"
# include "vccc/optional/internal/is_swappable.h"

namespace vccc {

template<typename T,
  std::enable_if_t<
    std::is_move_constructible<T>::value && internal::optional::is_swappable<T>::value,
    int> = 0>
inline void swap(optional<T>& lhs, optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

} // namespace vccc

# endif // VCCC_OPTIONAL_SWAP_H_
