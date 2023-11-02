# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
#ifndef VCCC_FUNCTIONAL_INVOKE_HPP
#define VCCC_FUNCTIONAL_INVOKE_HPP
#
#
# if __cplusplus <= 201402L
# include "vccc/functional/detail/invoke.hpp"

namespace vccc {

//! @addtogroup functional
//! @{

/**
 * @brief invokes any \a Callable object with given arguments
 * @param f         \a Callable object to be invoked
 * @param args      parameters
 * @return          result
 */

template<class F, class... Args>
constexpr inline
decltype(auto)
invoke(F&& f, Args&&... args) {
  return internal::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
}

//! @}

} // namespace vccc

# else
#
# include <functional>

namespace vccc {

template<class F, class... Args>
constexpr inline auto
invoke(F&& f, Args&&... args) {
  return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
}

} // namespace vccc

# endif

#endif // VCCC_FUNCTIONAL_INVOKE_HPP
