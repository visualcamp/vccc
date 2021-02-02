# /*
#   Created by YongGyu Lee on 2020/12/01.
# */
# ifndef VCCC_LOG_IOS_HPP
# define VCCC_LOG_IOS_HPP
#
# include "TargetConditionals.h"
# include <syslog.h>
#
# ifdef DEBUG
#     define LOGV_IMPL(...) syslog(LOG_NOTICE, __VA_ARGS__);
#     define LOGD_IMPL(...) syslog(LOG_DEBUG, __VA_ARGS__);
#     define LOGI_IMPL(...) syslog(LOG_INFO, __VA_ARGS__);
#     define LOGW_IMPL(...) syslog(LOG_WARNING, __VA_ARGS__);
#     define LOGF_IMPL(...) syslog(LOG_ERR, __VA_ARGS__);
#     define LOGS_IMPL(...) syslog(LOG_ERR, __VA_ARGS__);
# else
#     ifndef NDEBUG
#         define NDEBUG
#     endif
#
#     define LOGV_IMPL(...)
#     define LOGD_IMPL(...)
#     define LOGI_IMPL(...)
#     define LOGW_IMPL(...)
#     define LOGF_IMPL(...)
#     define LOGS_IMPL(...)
# endif
#
# define LOGE_IMPL(...) syslog(LOG_ERR, __VA_ARGS__);
#
# endif //VCCC_LOG_IOS_HPP
