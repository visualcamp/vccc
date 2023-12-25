//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_CORE_INLINE_OR_STATIC_HPP_
#define VCCC_CORE_INLINE_OR_STATIC_HPP_

#if __cplusplus < 201703L
#define VCCC_INLINE_OR_STATIC static
#else
#define VCCC_INLINE_OR_STATIC inline
#endif

#endif // VCCC_CORE_INLINE_OR_STATIC_HPP_
