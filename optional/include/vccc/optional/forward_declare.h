# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef VCCC_OPTIONAL_FORWARD_DECLARE_H_
# define VCCC_OPTIONAL_FORWARD_DECLARE_H_

namespace vccc {

template<typename T>
class optional;

template<typename T>
inline void swap(optional<T>& lhs, optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs)));


} // namespace vccc

# endif // VCCC_OPTIONAL_FORWARD_DECLARE_H_
