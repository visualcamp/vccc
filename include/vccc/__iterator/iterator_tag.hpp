//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_ITERATOR_ITERATOR_TAG_HPP_
#define VCCC_ITERATOR_ITERATOR_TAG_HPP_

#include <iterator>

namespace vccc {

/// @addtogroup iterator
/// @{

using input_iterator_tag = std::input_iterator_tag;
using output_iterator_tag = std::output_iterator_tag;
using forward_iterator_tag = std::forward_iterator_tag;
using bidirectional_iterator_tag = std::bidirectional_iterator_tag;
using random_access_iterator_tag = std::random_access_iterator_tag;

#if __cplusplus < 202002L
struct contiguous_iterator_tag : public random_access_iterator_tag {};
#else
using contiguous_iterator_tag = std::contiguous_iterator_tag;
#endif

// placeholder for ignoring typedefs
struct iterator_ignore : input_iterator_tag {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_ITERATOR_TAG_HPP_
