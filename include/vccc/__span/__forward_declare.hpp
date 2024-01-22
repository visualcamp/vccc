//
// Created by yonggyulee on 1/23/24.
//

#ifndef VCCC_SPAN___FORWARD_DECLARE_HPP
#define VCCC_SPAN___FORWARD_DECLARE_HPP

#include <cstddef>

#include "vccc/__span/dynamic_extent.hpp"

namespace vccc {

template<typename T, std::size_t Extent = dynamic_extent>
class span;

} // namespace vccc

#endif // VCCC_SPAN___FORWARD_DECLARE_HPP
