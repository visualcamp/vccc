//
// Created by yonggyulee on 2024/01/04.
//

#ifndef VCCC_CORE_NO_UNIQUE_ADDRESS_HPP
#define VCCC_CORE_NO_UNIQUE_ADDRESS_HPP

#if __cplusplus < 202002L
#define VCCC_NO_UNIQUE_ADDRESS
#else
#define VCCC_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

#endif // VCCC_CORE_NO_UNIQUE_ADDRESS_HPP
