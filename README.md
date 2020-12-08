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
 * `VCCC_REQUIRE`, `VCCC_REQUIRE_FORWARD`, `VCCC_REQUIRE_IMPL`
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
Standalone  
 * `make_rindex_sequence`, `make_same_sequence`, `make_zero_sequence`, `make_range_sequence`
 * `getCurrentMilliseconds`, `getCurrentMicroseconds`, `getCurrentNanoseconds`
 * `EXPECTS`, `ENSURES`, `BOUNDS_ASSERT`, `ALWAYS_ASSERT`
 
 #### `type_support`
 Requires `OpenCV`, `type_traits`, `utility`
 * `at`, `bind_at`
 * `convert_to`
 * `vtype_convert`
 * `resize`
 * `add`
 * `reserved_vector`
 
