//
// Created by yonggyulee on 2024/01/22.
//

#ifndef VCCC_RANGES_VIEWS_ELEMENTS_HPP
#define VCCC_RANGES_VIEWS_ELEMENTS_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

class elements_adaptor_object : public range_adaptor_closure<elements_adaptor_object> {

  template<std::size_t N>
  constexpr auto operator()() const {

  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::elements_adaptor_object as_const{};

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_ELEMENTS_HPP
