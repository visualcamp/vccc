# /*
#  * Created by YongGyu Lee on 2021/01/06.
#  */
#
# ifndef VCCC_LOG_FUNCTION_MACRO_HPP
# define VCCC_LOG_FUNCTION_MACRO_HPP

//! @addtogroup log
//! @{

namespace vccc {
namespace internal {

/**
 * @brief test
 */
inline void function_name_test() {

# ifdef __PRETTY_FUNCTION__
#   define VCCC_FUNCTION __PRETTY_FUNCTION__
# elif defined(__FUNCSIG__)
#   define VCCC_FUNCTION __FUNCSIG__
# elif defined(__func__)
#   define VCCC_FUNCTION __func__
# else
#   define VCCC_FUNCTION ("unknown")
# endif

}

} // namespace internal
} // namespace vccc

/**
@brief `VCCC_FUNCTION`: platform-wide function name macro

Use `VCCC_FUNCTION` instead of `VCCC_FUNCTION_DOXYGEN_HELPER`.<br>
 (lack of doxygen preprocessing parser)

Output is compiler-specific.<br>
`VCCC_FUNCTION` is equal to one of the followings
 - `__PRETTY_FUNCTION__`
 - `__FUNCSIG__`
 - `__func__`
 - `"unknown"`

@code{.cpp}
int main(){
  std::cout << VCCC_FUNCTION << std::endl; // prints something like: int main()
  return 0;
}
@endcode

*/
# define VCCC_FUNCTION_DOXYGEN_HELPER

//! @} log

# endif // VCCC_LOG_FUNCTION_MACRO_HPP
