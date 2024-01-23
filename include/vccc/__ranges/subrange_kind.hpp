//
// Created by yonggyulee on 2024/01/04.
//

#ifndef VCCC_RANGES_SUBRANGE_KIND_HPP
#define VCCC_RANGES_SUBRANGE_KIND_HPP

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{

enum
#if !defined(_MSC_VER) || __cplusplus >= 201703L
class
#endif
subrange_kind : bool {
    unsized,
    sized
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_SUBRANGE_KIND_HPP
