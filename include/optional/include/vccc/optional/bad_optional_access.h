# /*
#  * Created by YongGyu Lee on 2021/05/23.
#  */
#
# ifndef VCCC_OPTIONAL_BAD_OPTIONAL_ACCESS_H_
# define VCCC_OPTIONAL_BAD_OPTIONAL_ACCESS_H_
#
# include <exception>

namespace vccc {

//! @addtogroup optional
//! @{

/**
 * @brief exception indicating checked access to an optional that doesn't contain a value
 */
class bad_optional_access : public std::exception {
 public:
  bad_optional_access() noexcept = default;
  bad_optional_access(const bad_optional_access&) noexcept = default;
  bad_optional_access& operator=(const bad_optional_access&) noexcept = default;

  const char *what() const noexcept override {
    return "bad optional access";
  }
};

//! @} optional

} // namespace vccc

# endif // VCCC_OPTIONAL_BAD_OPTIONAL_ACCESS_H_
