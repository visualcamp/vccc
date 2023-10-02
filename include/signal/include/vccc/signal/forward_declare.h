# /*
#  * Created by YongGyu Lee on 2021/06/03.
#  */
#
# ifndef VCCC_SIGNAL_FORWARD_DECLARE_H_
# define VCCC_SIGNAL_FORWARD_DECLARE_H_

namespace vccc {

class signal_impl_base;

template<typename F, typename Group>
class signal_impl;

template<typename F, typename Group = int>
class signal;

} // namespace vccc

# endif // VCCC_SIGNAL_FORWARD_DECLARE_H_
