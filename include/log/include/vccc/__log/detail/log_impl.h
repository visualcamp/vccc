# /*
#   Created by YongGyu Lee on 2021/04/05.
# */
#
# ifndef VCCC_LOG_DETAIL_LOG_IMPL_H_
# define VCCC_LOG_DETAIL_LOG_IMPL_H_
#
# include "vccc/__log/detail/buffer.h"
#
# ifdef NDEBUG
#   define LOGD_IMPL(tag, fmt, ...)
# else
#   define LOGD_IMPL(tag, fmt, ...) VCCC_LOG_EXPAND(VCCC_LOG_PRINTER(VCCC_LOG_BUFFER_DEBUG, tag, fmt, __VA_ARGS__))
# endif
#
# define LOGI_IMPL(tag, fmt, ...) VCCC_LOG_EXPAND(VCCC_LOG_PRINTER(VCCC_LOG_BUFFER_INFO, tag, fmt, __VA_ARGS__))
# define LOGW_IMPL(tag, fmt, ...) VCCC_LOG_EXPAND(VCCC_LOG_PRINTER(VCCC_LOG_BUFFER_WARN, tag, fmt, __VA_ARGS__))
# define LOGE_IMPL(tag, fmt, ...) VCCC_LOG_EXPAND(VCCC_LOG_PRINTER(VCCC_LOG_BUFFER_ERROR, tag, fmt, __VA_ARGS__))
#
# endif //VCCC_LOG_DETAIL_LOG_IMPL_H_
