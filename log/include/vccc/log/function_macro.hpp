# /*
#  * Created by YongGyu Lee on 2021/01/06.
#  */
#
# ifndef VCCC_LOG_FUNCTION_MACRO_HPP
# define VCCC_LOG_FUNCTION_MACRO_HPP



//! @addtogroup log
//! @{


#ifdef __PRETTY_FUNCTION__
#define VCCC_FUNCTION __PRETTY_FUNCTION__
#else
#define VCCC_FUNCTION __FUNCTION__
#endif

 //! @} log


# endif //VCCC_LOG_FUNCTION_MACRO_HPP
