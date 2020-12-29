# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_LOG_PWD_HPP
# define VCCC_LOG_PWD_HPP
#
# include <string>

//! @addtogroup log
//! @{

/**
@brief get current working directory

returns current calling file's working directory. <br>
This macro must be directly used in code where needed, or it will generate wrong output
 */
#define PWD                                     \
[](){                                           \
  std::string $13vlaae_dummy_str = __FILE__;    \
  return $13vlaae_dummy_str.substr(             \
    0, $13vlaae_dummy_str.find_last_of('/'));   \
}()

//! @} log


# endif //VCCC_LOG_PWD_HPP
