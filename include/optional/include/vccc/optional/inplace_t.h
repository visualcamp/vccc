# /*
#  * Created by YongGyu Lee on 2021/05/23.
#  */
#
# ifndef VCCC_OPTIONAL_INPLACE_T_H_
# define VCCC_OPTIONAL_INPLACE_T_H_

namespace vccc {

//! @addtogroup optional
//! @{

/**
 * @brief in-place construction tag
 */
struct in_place_t {
  constexpr explicit in_place_t() = default;
};

constexpr in_place_t in_place{};

//! @} optional

} // namespace vccc

# endif // VCCC_OPTIONAL_INPLACE_T_H_
