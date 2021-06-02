# /*
#  * Created by YongGyu Lee on 2021/06/03.
#  */
#
# ifndef VCCC_EXPERIMENTAL_SIGNAL_FORWARD_DECLARE_H_
# define VCCC_EXPERIMENTAL_SIGNAL_FORWARD_DECLARE_H_

namespace vccc {
namespace experimental {
namespace signal {

class signal_impl_base;

template<typename F, typename Group>
class signal_impl;

template<typename F, typename Group = int>
class signal;

} // namespace signal
} // namespace experimental
} // namespace vccc

# endif // VCCC_EXPERIMENTAL_SIGNAL_FORWARD_DECLARE_H_
