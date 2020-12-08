## vccc/type_traits

#### `are_<...>`  
* helper templates
* `are_arithmetic<...>`, `are_integral<...>`, `are_floating_point<...>`, `are_scalar<...>`  

conditions can be unpacked inside `are<...>`.  
ex)
```
template<typename ...Ts>
using are_integral_t = are_<std::is_integral<Ts>...>::type;

std::cout << are_integral_t<int, int, int>{} << std::endl;
```
<br></br>

#### `bigger_type`, `signed_bigger_type`, `unsigned_bigger_type`


<br></br>
#### `is_container`


<br></br>
#### `is_printable`


<br></br>
#### `is_specialization`


<br></br>
#### `is_std_array`


<br></br>
#### `is_iterator`, `iterable`


<br></br>
#### `lossless_type_add`,`lossless_type_mul`,`lossless_type_div`


<br></br>
#### `multiples_of`, `is_odd`, `is_even`


<br></br>
#### `VCCC_REQUIRE`, `VCCC_REQUIRE_FORWARD`, `VCCC_REQUIRE_IMPL`


<br></br>
#### `void_t`


<br></br>
#### `vtype_t`
