//
// Created by YongGyu Lee on 2023/03/31.
//

#ifndef VCCC_LOG_IOMANIP_HPP
#define VCCC_LOG_IOMANIP_HPP

#include <string>

#include "vccc/__log/stream_wrapper.hpp"

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

/// @brief Manipulator for adding quotation to strings(only to string and string_view)
class Quoted : public StreamManipulator {
 public:
  Quoted() : value_(true) {}

  explicit Quoted(bool value) : value_(value) {}


  template<typename CharT, typename String, typename Stream>
  BasicStreamWrapper<CharT, String, Stream>& operator()(BasicStreamWrapper<CharT, String, Stream>& stream) const {
    stream.quote_string(value_);
    return stream;
  }

 private:
  bool value_;
};

/// @brief Manipulator for expanding aggregate types
///
/// Note: empty aggregate will not be expanded
class ExpandAggregate : public StreamManipulator {
 public:
  ExpandAggregate() : value_(true) {}

  explicit ExpandAggregate(bool value) : value_(value) {}

  template<typename CharT, typename String, typename Stream>
  BasicStreamWrapper<CharT, String, Stream>& operator()(BasicStreamWrapper<CharT, String, Stream>& stream) const {
    stream.expand_aggregate(value_);
    return stream;
  }

 private:
  bool value_;
};

/// @brief Expand array instead of printing its value
///
/// Note: non-default-printable array is expanded by default
class ExpandArray : public StreamManipulator {
 public:
  ExpandArray() : value_(true) {}

  explicit ExpandArray(bool value) : value_(value) {}

  template<typename CharT, typename String, typename Stream>
  BasicStreamWrapper<CharT, String, Stream>& operator()(BasicStreamWrapper<CharT, String, Stream>& stream) const {
    stream.expand_array(value_);
    return stream;
  }

 private:
  bool value_;
};

//! @} log

} // namespace vccc

#endif // VCCC_LOG_IOMANIP_HPP
