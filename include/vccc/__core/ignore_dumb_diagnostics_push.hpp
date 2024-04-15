//
// Created by YongGyu Lee on 4/12/24.
//

#ifndef VCCC_CORE_IGNORE_DUMB_DIAGNOSTICS_PUSH_HPP_
#define VCCC_CORE_IGNORE_DUMB_DIAGNOSTICS_PUSH_HPP_

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-internal"

#elif defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 5046)
#endif

#endif // VCCC_CORE_IGNORE_DUMB_DIAGNOSTICS_PUSH_HPP_
