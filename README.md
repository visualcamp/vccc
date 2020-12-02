 # VCCC
 ## VisualCamp Common C++ library
 * Requires C++14 or above
 
 #### `literal`
 Standalone
 * `_format`
 * `Formatter`
 
 #### `type_traits`
 Standalone  
 * `are_`, `are_...`
 * `vtype_t`, `void_t`, `is_iterator`, `iterable`, `is_specialization`, `is_container`, `is_printable`, `is_std_array` 
 * `bigger_type`, `signed_bigger_type`, `unsigned_bigger_type`
 * `VCPP_REQUIRE`, `VCPP_REQUIRE_FWD`, `VCPP_REQUIRE_IMPL`
 * `multiples_of`, `is_odd`
 
 #### `log`
 Requires `type_traits`  
 * `LOGD`, `LOGE`, ...
 * `PWD`
 
 #### `numeric`
 Requires `type_traits`  
 * `sum`, `square`, `square_sum`
 * `average`, `stddev`, `avg_stddev`
 
 #### `utility`
 Requires `type_traits`  
 Requires `OpenCV for` for full use
 * `at`, `bind_at`
 * `make_rindex_sequence`, `make_same_sequence`, `make_zero_sequence`, `make_range_sequence`
 * `getCurrentMilliseconds`, `getCurrentMicroseconds`, `getCurrentNanoseconds`
 
 #### `type_support`
 Requires `OpenCV`, `type_traits`, `utility`
 * `convert_to`
 * `vtype_convert`
 * `resize`
 * `add`
 * `reserved_vector`
 
