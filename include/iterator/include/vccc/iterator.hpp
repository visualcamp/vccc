//
// Created by yonggyulee on 2023/12/23.
//

#ifndef VCCC_ITERATOR_HPP_
#define VCCC_ITERATOR_HPP_

#include "vccc/__iterator/detail/iter_concept.hpp"
#include "vccc/__iterator/detail/iter_move_std.hpp"

#include "vccc/__iterator/iterator_traits/forward_declare.hpp"
#include "vccc/__iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/__iterator/iterator_traits/legacy_bidirectional_iterator.hpp"
#include "vccc/__iterator/iterator_traits/legacy_forward_iterator.hpp"
#include "vccc/__iterator/iterator_traits/legacy_input_iterator.hpp"
#include "vccc/__iterator/iterator_traits/legacy_iterator.hpp"
#include "vccc/__iterator/iterator_traits/legacy_random_access_iterator.hpp"

#include "vccc/__iterator/advance.hpp"
#include "vccc/__iterator/bidirectional_iterator.hpp"
#include "vccc/__iterator/contiguous_iterator.hpp"
#include "vccc/__iterator/counted_iterator.hpp"
#include "vccc/__iterator/default_sentinel_t.hpp"
#include "vccc/__iterator/forward_iterator.hpp"
#include "vccc/__iterator/incrementable.hpp"
#include "vccc/__iterator/incrementable_traits.hpp"
#include "vccc/__iterator/indirect_binary_predicate.hpp"
#include "vccc/__iterator/indirect_result_t.hpp"
#include "vccc/__iterator/indirect_strict_weak_order.hpp"
#include "vccc/__iterator/indirect_unary_predicate.hpp"
#include "vccc/__iterator/indirectly_comparable.hpp"
#include "vccc/__iterator/indirectly_copyable.hpp"
#include "vccc/__iterator/indirectly_copyable_storable.hpp"
#include "vccc/__iterator/indirectly_movable.hpp"
#include "vccc/__iterator/indirectly_movable_storable.hpp"
#include "vccc/__iterator/indirectly_readable.hpp"
#include "vccc/__iterator/indirectly_readable_traits.hpp"
#include "vccc/__iterator/indirectly_regular_unary_invocable.hpp"
#include "vccc/__iterator/indirectly_swappable.hpp"
#include "vccc/__iterator/indirectly_unary_invocable.hpp"
#include "vccc/__iterator/indirectly_writable.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/input_or_output_iterator.hpp"
#include "vccc/__iterator/iter_common_reference_t.hpp"
#include "vccc/__iterator/iter_const_reference_t.hpp"
#include "vccc/__iterator/iter_difference_t.hpp"
#include "vccc/__iterator/iter_move.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__iterator/iter_rvalue_reference_t.hpp"
#include "vccc/__iterator/iter_swap.hpp"
#include "vccc/__iterator/iter_value_t.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__iterator/output_iterator.hpp"
#include "vccc/__iterator/prev.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__iterator/random_access_iterator.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__iterator/sized_sentinel_for.hpp"
#include "vccc/__iterator/unreachable_sentinel.hpp"
#include "vccc/__iterator/weakly_incrementable.hpp"

#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/data.hpp"
#include "vccc/__ranges/empty.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/size.hpp"
#include "vccc/__ranges/ssize.hpp"

/**
@defgroup iterator iterator
@brief Iterators
*/

#endif // VCCC_ITERATOR_HPP_
