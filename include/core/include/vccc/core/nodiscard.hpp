//
// Created by yonggyulee on 2024/01/04.
//

#ifndef VCCC_CORE_NODISCARD_HPP
#define VCCC_CORE_NODISCARD_HPP

#if __cplusplus < 201703L
#define VCCC_NODISCARD
#else
#define VCCC_NODISCARD [[nodiscard]]
#endif

#endif // VCCC_CORE_CONSTEXPR_HPP
