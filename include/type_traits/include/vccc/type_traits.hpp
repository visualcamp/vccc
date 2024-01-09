# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_HPP
# define VCCC_TYPE_TRAITS_HPP
#
# include "vccc/__type_traits/core/size.hpp"

# include "vccc/__type_traits/detail/is_swappable_cxx14.hpp"
# include "vccc/__type_traits/detail/lossless_type_int_division.hpp"
# include "vccc/__type_traits/detail/return_category.hpp"
# include "vccc/__type_traits/detail/tag.hpp"
# include "vccc/__type_traits/detail/test_ternary.hpp"

# include "vccc/__type_traits/are.hpp"
# include "vccc/__type_traits/arity.hpp"
# include "vccc/__type_traits/bigger_type.hpp"
# include "vccc/__type_traits/bool_constant.hpp"
# include "vccc/__type_traits/common_reference.hpp"
# include "vccc/__type_traits/common_type.hpp"
# include "vccc/__type_traits/conjunction.hpp"
# include "vccc/__type_traits/disjunction.hpp"
# include "vccc/__type_traits/empty.hpp"
# include "vccc/__type_traits/has_typename_difference_type.hpp"
# include "vccc/__type_traits/has_typename_element_type.hpp"
# include "vccc/__type_traits/has_typename_type.hpp"
# include "vccc/__type_traits/has_typename_value_type.hpp"
# include "vccc/__type_traits/is_bounded_array.hpp"
# include "vccc/__type_traits/is_complete.hpp"
# include "vccc/__type_traits/is_explicitly_convertible.hpp"
# include "vccc/__type_traits/is_initializer_list.hpp"
# include "vccc/__type_traits/is_integer_like.hpp"
# include "vccc/__type_traits/is_invocable.hpp"
# include "vccc/__type_traits/is_printable.hpp"
# include "vccc/__type_traits/is_range.hpp"
# include "vccc/__type_traits/is_referenceable.hpp"
# include "vccc/__type_traits/is_printable.hpp"
# include "vccc/__type_traits/is_specialization.hpp"
# include "vccc/__type_traits/is_string_like.hpp"
# include "vccc/__type_traits/is_swappable.hpp"
# include "vccc/__type_traits/is_tuple_like.hpp"
# include "vccc/__type_traits/is_unbounded_array.hpp"
# include "vccc/__type_traits/iterable.hpp"
# include "vccc/__type_traits/lossless_type.hpp"
# include "vccc/__type_traits/negation.hpp"
# include "vccc/__type_traits/numeric.hpp"
# include "vccc/__type_traits/remove_cvref.hpp"
# include "vccc/__type_traits/type_identity.hpp"
# include "vccc/__type_traits/void_t.hpp"

/**
@defgroup type_traits type_traits
@brief Implementation of STL header
[`<type_traits>`](https://en.cppreference.com/w/cpp/header/type_traits) with some extra features
*/

# endif // VCCC_TYPE_TRAITS_HPP
