//
// Created by yonggyulee on 2023/12/30.
//

#ifndef VCCC_RANGES___FORWARD_DECLARE_HPP
#define VCCC_RANGES___FORWARD_DECLARE_HPP

namespace vccc {
namespace ranges {

template<typename R> struct range;
template<typename R> struct borrowed_range;
template<typename R> struct sized_range;
template<typename R> struct view;
template<typename R> struct input_range;
template<typename R> struct output_range;
template<typename R> struct forward_range;
template<typename R> struct bidirectional_range;
template<typename R> struct random_access_range;
template<typename R> struct contiguous_range;
template<typename R> struct common_range;
template<typename R> struct viewable_range;
template<typename R> struct constant_range;

template<typename Derived>
class view_interface;

namespace views {


} // namespace views
} // namespace ranges

namespace views = ranges::views;

} // namespace vccc

#endif // VCCC_RANGES___FORWARD_DECLARE_HPP
