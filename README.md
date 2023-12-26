<p align="center">
  <img src="/docs/image/vc_logo_original.png"></img></br>
</p>

![develop](https://github.com/visualcamp/vccc/actions/workflows/test-macos.yml/badge.svg?branch=main)
![develop](https://github.com/visualcamp/vccc/actions/workflows/test-ubuntu.yml/badge.svg?branch=main)
![develop](https://github.com/visualcamp/vccc/actions/workflows/test-windows.yml/badge.svg?branch=main)

[![Deploy](https://github.com/visualcamp/vccc/actions/workflows/deploy-docs.yml/badge.svg)](https://github.com/visualcamp/vccc/actions/workflows/deploy-docs.yml)

# VisualCamp Common C++ library
## A header-only cross-platform library that implements C++17, C++20, C++23 STL using C++14
#### Example, 1
```.cpp
#include "vccc/ranges.hpp"
#include "vccc/log.hpp"

for (auto x : vccc::views::iota(0, 10)) {
  LOGI(x);
}

// Constraints are implemented
// vccc::views::iota(0, 10U) <- compile failes on the immediate context
```
#### Output
* Outputs are consistent on every platform(PC, web browser, Android and iOS)
```.cpp
0
1
2
...
9
```

#### Example, 2
```.cpp
#include "vccc/log.hpp"

std::vector<int> v = {1, 2, 3, 4};
std::map<int, std::string> map = {{1, "one"}, {2, "two"}};
auto t = std::make_tuple(1, "hello", v);

LOGI(v);
LOGI(map);
LOGI(t);
```
#### Output
```.cpp
{1, 2, 3, 4}
{{1: "one"}, {2: "two"}}
{1, "hello", {1, 2, 3, 4}}
```

## Compiler Support
* GCC 6.0 or later
* Clang 8.0 or later
* MSVC 15.0 or later
* Emscripten 2.0.14 or later (pervious versions may work but not tested)
* Android NDK 21.1.6352462 or later (pervious versions may work but not tested)
* iOS

## Requirements
* C++14 or above
* Boost.predef, Boost.pfr


## [Go to document](https://visualcamp.github.io/vccc/html/index.html)
