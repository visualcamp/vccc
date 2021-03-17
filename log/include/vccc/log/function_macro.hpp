# /*
#  * Created by YongGyu Lee on 2021/01/06.
#  */
#
# ifndef VCCC_LOG_FUNCTION_MACRO_HPP
# define VCCC_LOG_FUNCTION_MACRO_HPP
#
# include "boost/predef.h"

//! @addtogroup log
//! @{


# if BOOST_COMP_MSVC != BOOST_VERSION_NUMBER_NOT_AVAILABLE
#     define VCCC_FUNCTION __FUNCSIG__
# else
#     define VCCC_FUNCTION __PRETTY_FUNCTION__
# endif

 //! @} log


# endif //VCCC_LOG_FUNCTION_MACRO_HPP
