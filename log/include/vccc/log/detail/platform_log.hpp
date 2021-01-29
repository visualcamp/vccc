# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_LOG_PLATFORM_LOG
# define VCCC_LOG_PLATFORM_LOG
#
# include "boost/predef.h"
#
# if BOOST_PLAT_ANDROID != BOOST_VERSION_NUMBER_NOT_AVAILABLE
#     include "vccc/log/detail/android.hpp"
# elif BOOST_OS_IOS != BOOST_VERSION_NUMBER_NOT_AVAILABLE
#     include "vccc/log/detail/ios.hpp"
# else
#     include "vccc/log/detail/pc.hpp"
# endif
#
# endif //VCCC_LOG_PLATFORM_LOG
