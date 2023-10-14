//
// Created by YongGyu Lee on 2023/03/31.
//

#ifndef VCCC_LOG_IOMANIP_HPP
#define VCCC_LOG_IOMANIP_HPP

#include <string>

#include "vccc/log/stream_wrapper.hpp"

namespace vccc {

//! @addtogroup log
//! @{

/** @brief Set separator to vccc::BasicStreamWrapper

@code{.cpp}
    vccc::StreamWrapper stream;
    stream << 1 << 2 << 3; // stream.str() == "123"

    stream.str("");
    stream << vccc::Separator("FOO") << 1 << 2 << 3; // stream.str() == "1FOO2FOO3"
@endcode
 */
class Separator : public StreamManipulator {
 public:
  explicit Separator(std::string sep) : sep_(std::move(sep)) {}

  template<typename CharT, typename String, typename Stream>
  BasicStreamWrapper<CharT, String, Stream>& operator()(BasicStreamWrapper<CharT, String, Stream>& stream) const {
    stream.separator(sep_);
    return stream;
  }

 private:
  std::string sep_;
};

//! @} log

} // namespace vccc

#endif // VCCC_LOG_IOMANIP_HPP
