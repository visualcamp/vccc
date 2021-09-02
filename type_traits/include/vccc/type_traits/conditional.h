# /*
#  * Created by YongGyu Lee on 2021/09/03.
#  */
#
# ifndef VCCC_TYPE_TRAITS_CONDITIONAL_H_
# define VCCC_TYPE_TRAITS_CONDITIONAL_H_

namespace vccc {


/**
 * conditional
 * @tparam b
 * @tparam If
 * @tparam Then
 */
template<bool b, typename If, typename Then>
struct conditional { using type = If; };

template<typename If, typename Then>
struct conditional<false, If, Then> { using type = Then; };

template<bool b, typename If, typename Then>
using conditional_t = typename conditional<b, If, Then>::type;

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_CONDITIONAL_H_
