# /*
#   Created by YongGyu Lee on 2020/12/01.
# */
# ifndef VCCC_LOG_PC_HPP
# define VCCC_LOG_PC_HPP
#
# include <cstdio>
#
# ifdef NDEBUG
#     define LOGV_IMPL(...)
#     define LOGD_IMPL(...)
#     define LOGI_IMPL(...)
#     define LOGW_IMPL(...)
#     define LOGE_IMPL(...)
#     define LOGF_IMPL(...)
#     define LOGS_IMPL(...)
# else
#     define VCCC_DETAIL_EXPAND(x) x // defense for MSVC
#
#     define PRINT_WITH_NEWLINE(fmt, ...)       VCCC_DETAIL_EXPAND(std::printf(fmt "\n", __VA_ARGS__))
#     define FPRINT_WITH_NEWLINE(out, fmt, ...) VCCC_DETAIL_EXPAND(std::fprintf(out, fmt "\n", __VA_ARGS__))
#
#     define LOGV_IMPL(...) VCCC_DETAIL_EXPAND(FPRINT_WITH_NEWLINE(stdout, __VA_ARGS__))
#     define LOGD_IMPL(...) VCCC_DETAIL_EXPAND(FPRINT_WITH_NEWLINE(stdout, __VA_ARGS__))
#     define LOGI_IMPL(...) VCCC_DETAIL_EXPAND(FPRINT_WITH_NEWLINE(stdout, __VA_ARGS__))
#     define LOGW_IMPL(...) VCCC_DETAIL_EXPAND(FPRINT_WITH_NEWLINE(stdout, __VA_ARGS__))
#     define LOGE_IMPL(...) VCCC_DETAIL_EXPAND(FPRINT_WITH_NEWLINE(stderr, __VA_ARGS__))
#     define LOGF_IMPL(...) VCCC_DETAIL_EXPAND(FPRINT_WITH_NEWLINE(stdout, __VA_ARGS__))
#     define LOGS_IMPL(...) VCCC_DETAIL_EXPAND(FPRINT_WITH_NEWLINE(stdout, __VA_ARGS__))
# endif
#
# endif //VCCC_LOG_PC_HPP
