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

namespace vccc{

//! @addtogroup functional
//! @{

/**
@brief invokes the callable object f with the parameter args
 * @param f         callable object
 * @param args      parameters
 * @return          result
 */

template<class F, class... Args>
constexpr inline
decltype(auto)
invoke(F&& f, Args&&... args) {
  return detail::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
}

//! @}

}

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
