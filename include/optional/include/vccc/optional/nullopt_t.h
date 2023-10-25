# /*
#  * Created by YongGyu Lee on 2021/05/23.
#  */
#
# ifndef VCCC_OPTIONAL_NULLOPT_T_H_
# define VCCC_OPTIONAL_NULLOPT_T_H_

namespace vccc {

//! @addtogroup optional
//! @{

/**
 * @brief indicator of optional type with uninitialized state
 *
 * See [std::nullopt_t](https://en.cppreference.com/w/cpp/utility/optional/nullopt_t)
 * for more information
 */
struct nullopt_t {
  constexpr explicit nullopt_t(int) {}
};

constexpr nullopt_t nullopt{0};

//! @} optional

} // namespace vccc

# endif // VCCC_OPTIONAL_NULLOPT_T_H_
