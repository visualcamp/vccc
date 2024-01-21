//
// Created by yonggyulee on 2024/01/04.
//

#ifndef VCCC_CORE_NODISCARD_HPP
#define VCCC_CORE_NODISCARD_HPP

#if __cplusplus < 201703L
#  if defined(__GNUC__) && (__GNUC__ >= 4)
#    define VCCC_NODISCARD __attribute__ ((warn_unused_result))
#  elif defined(_MSC_VER) && (_MSC_VER >= 1700)
#    define VCCC_NODISCARD _Check_return_
#  else
#    define VCCC_NODISCARD
#  endif
#else
#  define VCCC_NODISCARD [[nodiscard]]
#endif

#endif // VCCC_CORE_CONSTEXPR_HPP
