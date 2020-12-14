## vccc/literal

#### `Formatter`
* helper literals `operator "" _format`

`constructor (const char *format)`  
`std::string operator()(const Args& ...args)`

ex)
```
using namespace vccc;

std::cout << "hello, %s"_format("world!") << std::endl; // hello, world!

Formatter fmt("hello, %s");
std::cout << fmt("world!") << std::endl; // hello, world!
```