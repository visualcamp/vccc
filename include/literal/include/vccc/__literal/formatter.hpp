# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_LITERAL_FORMATTER_HPP
# define VCCC_LITERAL_FORMATTER_HPP
#
# include <string>
# include <vector>
# include <cstdio>

namespace vccc {

//! @addtogroup literal
//! @{

/**
@brief converts c-style format and inputs to std::string

Formatting helper class & custom literals

@code{.cpp}
    auto str = vccc::Formatter("x:%d, hello %s")(13, "world!");
@endcode
 */
class Formatter {
 public:
  explicit Formatter(const char *format) : format(format), buffer(30) {}

  template<typename ...Args>
  inline std::string operator()(const Args& ...args) {
    auto size = snprintf(NULL, 0, format, args...);
    if (buffer.size() < size + 1)
      buffer.resize(size + 1);
    snprintf(buffer.data(), size + 1, format, args...);
    return std::string(buffer.data());
  }

 private:
  const char *format;
  std::vector<char> buffer;
};


/** @brief Formatter helper on string literal

@param format    c-style string literal format
@return          Formatter

@code{.cpp}
    std::string str = "x:%d, hello %s"_format(13, "world!");

    vccc::Formatter fmt = "%s"_format;
    std::string str2 = fmt("Hello, world!");
@endcode

 */

inline Formatter operator "" _format(const char *format, std::size_t) {
  return Formatter(format);
}

//! @} literal

} // namespace vccc

# endif // VCCC_LITERAL_FORMATTER_HPP
