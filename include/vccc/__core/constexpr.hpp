//
// Created by yonggyulee on 2024/01/03.
//

#ifndef VCCC_CORE_CONSTEXPR_HPP
#define VCCC_CORE_CONSTEXPR_HPP

// TODO: Use CheckCXXSybolExists
#define VCCC_CONSTEXPR constexpr

#if __cplusplus >= 201402L
#define VCCC_CONSTEXPR_AFTER_CXX14 constexpr
#else
#define VCCC_CONSTEXPR_AFTER_CXX14
#endif

#if __cplusplus >= 201703L
#define VCCC_CONSTEXPR_AFTER_CXX17 constexpr
#else
#define VCCC_CONSTEXPR_AFTER_CXX17
#endif

#if __cplusplus >= 202002L
#define VCCC_CONSTEXPR_AFTER_CXX20 constexpr
#else
#define VCCC_CONSTEXPR_AFTER_CXX20
#endif

#if __cplusplus >= 202302L
#define VCCC_CONSTEXPR_AFTER_CXX23 constexpr
#else
#define VCCC_CONSTEXPR_AFTER_CXX23
#endif

#endif // VCCC_CORE_CONSTEXPR_HPP
