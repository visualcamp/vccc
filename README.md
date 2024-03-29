<p align="center">
  <img src="/docs/image/vc_logo_original.png"></img></br>
</p>

![develop](https://github.com/visualcamp/vccc/actions/workflows/test-macos.yml/badge.svg?branch=main)
![develop](https://github.com/visualcamp/vccc/actions/workflows/test-ubuntu.yml/badge.svg?branch=main)
![develop](https://github.com/visualcamp/vccc/actions/workflows/test-windows.yml/badge.svg?branch=main)
![develop](https://github.com/visualcamp/vccc/actions/workflows/test-android.yml/badge.svg?branch=main)
![develop](https://github.com/visualcamp/vccc/actions/workflows/test-emscripten.yml/badge.svg?branch=main)

[![Deploy](https://github.com/visualcamp/vccc/actions/workflows/deploy-docs.yml/badge.svg)](https://github.com/visualcamp/vccc/actions/workflows/deploy-docs.yml)

# VisualCamp Common C++ library
## Features
* algorithm library implemented in C++14 (up to C++23 standard)
* concepts library implemented in C++14 (up to C++23 standard)
* ranges library implemented in C++14 (up to C++26 standard)
* utility library implemented in c++14 (up to C++26 standard)
  * `expected`
  * `optional`
  * `span`
  * `variant`
* Other ~ C++26 libraries

#### All features are compatible with existing STL

## Examples
```c++
// All features available in C++14

#include "vccc/ranges.hpp"

// Deduced as std::vector<int>
auto v = vccc::views::iota(10)
       | vccc::views::take(4)
       | vccc::ranges::to<std::vector>();

std::pair<int, std::string> parr[] = {{1: "one"}, {2: "two"}};

// Deduced as std::map<int, std::string>
auto m = vccc::ranges::to<std::map>(parr); 

// Deduced to std::vector<std::pair<int, std::string>>
auto pv = vccc::ranges::to<std::vector>(parr);


auto arr = {'A', 'B', 'C', 'D'};
auto m = v | vccc::views::enumerate | vccc::ranges::to<std::map>();

#if __cplusplus < 201703L
    for (const auto kv : m)
      std::cout << '[' << std::get<0>(kv) << "]:" << std::get<1>(kv) << ' ';
#else
    for (auto const [key, value] : m)
      std::cout << '[' << key << "]:" << value << ' ';
#endif
```

## Requirements
* CMake
* C++ 14
* Boost.predef, Boost.pfr
* This is header-only. No further requirements

## Compiler Support
* GCC 6.0 or later
* Clang 8.0 or later
* MSVC 15.0 or later
* Emscripten 2.0.14 or later (pervious versions may work but not tested)
* Android NDK 21.1.6352462 or later (pervious versions may work but not tested)
* iOS


## [Go to document](https://visualcamp.github.io/vccc/html/index.html)
