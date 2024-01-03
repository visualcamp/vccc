//
// Created by yonggyulee on 2023/12/23.
//

#ifndef VCCC_ITERATOR_HPP_
#define VCCC_ITERATOR_HPP_

#include "vccc/iterator/detail/iter_concept.hpp"
#include "vccc/iterator/detail/iter_move_std.hpp"

#include "vccc/iterator/iterator_traits/forward_declare.hpp"
#include "vccc/iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/iterator/iterator_traits/legacy_bidirectional_iterator.hpp"
#include "vccc/iterator/iterator_traits/legacy_forward_iterator.hpp"
#include "vccc/iterator/iterator_traits/legacy_input_iterator.hpp"
#include "vccc/iterator/iterator_traits/legacy_iterator.hpp"
#include "vccc/iterator/iterator_traits/legacy_random_access_iterator.hpp"

#include "vccc/iterator/advance.hpp"
#include "vccc/iterator/bidirectional_iterator.hpp"
#include "vccc/iterator/contiguous_iterator.hpp"
#include "vccc/iterator/counted_iterator.hpp"
#include "vccc/iterator/default_sentinel_t.hpp"
#include "vccc/iterator/forward_iterator.hpp"
#include "vccc/iterator/incrementable.hpp"
#include "vccc/iterator/incrementable_traits.hpp"
#include "vccc/iterator/indirect_binary_predicate.hpp"
#include "vccc/iterator/indirect_result_t.hpp"
#include "vccc/iterator/indirect_strict_weak_order.hpp"
#include "vccc/iterator/indirectly_comparable.hpp"
#include "vccc/iterator/indirectly_readable.hpp"
#include "vccc/iterator/indirectly_readable_traits.hpp"
#include "vccc/iterator/indirectly_regular_unary_invocable.hpp"
#include "vccc/iterator/indirectly_unary_invocable.hpp"
#include "vccc/iterator/indirectly_writable.hpp"
#include "vccc/iterator/input_iterator.hpp"
#include "vccc/iterator/input_or_output_iterator.hpp"
#include "vccc/iterator/iter_common_reference_t.hpp"
#include "vccc/iterator/iter_const_reference_t.hpp"
#include "vccc/iterator/iter_difference_t.hpp"
#include "vccc/iterator/iter_move.hpp"
#include "vccc/iterator/iter_reference_t.hpp"
#include "vccc/iterator/iter_rvalue_reference_t.hpp"
#include "vccc/iterator/iter_value_t.hpp"
#include "vccc/iterator/iterator_tag.hpp"
#include "vccc/iterator/output_iterator.hpp"
#include "vccc/iterator/prev.hpp"
#include "vccc/iterator/projected.hpp"
#include "vccc/iterator/random_access_iterator.hpp"
#include "vccc/iterator/sentinel_for.hpp"
#include "vccc/iterator/sized_sentinel_for.hpp"
#include "vccc/iterator/unreachable_sentinel.hpp"
#include "vccc/iterator/weakly_incrementable.hpp"

#include "vccc/ranges/empty.hpp"

/**
@defgroup iterator iterator
@brief Iterators
*/

#endif // VCCC_ITERATOR_HPP_
