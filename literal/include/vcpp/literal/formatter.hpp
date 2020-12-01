//
// Created by YongGyu Lee on 2020/11/30.
//

#ifndef VCPP_LITERAL_FORMATTER_HPP
#define VCPP_LITERAL_FORMATTER_HPP

#include <string>
#include <vector>
#include <cstdio>

namespace vc {

/**
 *  Formatting helper class & custom literals
 *
 *  ex)
 *      auto str = "x:%d, hello %s"_format(13, "world!");
 *      LOGD("%.2f"_format(value), cv::Point2f(), std::vector<int>())
 */
class Formatter {
 public:
  explicit Formatter(const char *format) : format(format), buffer(30) {}

  template<typename ...Args>
  inline std::string operator()(const Args& ...args) {
    ssize_t size = snprintf(NULL, 0, format, args...);
    if (buffer.size() < size + 1)
      buffer.resize(size + 1);
    snprintf(buffer.data(), size + 1, format, args...);
    return std::string(buffer.data());
  }

 private:
  const char *format;
  std::vector<char> buffer;
};

inline Formatter operator "" _format(const char *format, std::size_t) {
  return Formatter(format);
}

}

#endif //VCPP_LITERAL_FORMATTER_HPP
