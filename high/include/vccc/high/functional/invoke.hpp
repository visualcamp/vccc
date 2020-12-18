# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
#ifndef VCCC_HIGH_FUNCTIONAL_INVOKE_HPP
#define VCCC_HIGH_FUNCTIONAL_INVOKE_HPP
#
#
# if __cplusplus <= 201402L
#
# include "vccc/high/functional/detail/invoke.hpp"

namespace vccc{

template<class F, class... Args>
constexpr
decltype(auto)
invoke(F&& f, Args&&... args) {
  return detail::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
}

}

# else
#
# include <functional>

namespace vccc{

template<class F, class... Args>
[[deprecated("use std::invoke instead")]]
constexpr
auto
invoke(F&& f, Args&&... args) {
  return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
}

}
# endif

#endif //VCCC_HIGH_FUNCTIONAL_INVOKE_HPP