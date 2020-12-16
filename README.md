 # VCCC
 ## VisualCamp Common C++ library
 * Requires C++14 or above
 
 #### `high`
 Standalone
 * `functional`
   * `invoke`
 * `tuple`
   * `apply`

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
 * `multiples_of`, `is_odd`, `is_even`
 * `lossless_type_add`, `lossless_type_mul`, `lossless_type_div`
 
 #### `log`
 Requires `type_traits`  
 * `LOGD`, `LOGE`, ...
 * `PWD`
 
 #### `numeric`
 Requires `type_traits`  
 * `average`
 * `lossless_div`
 * `norm`
 * `stddev`, `avg_stddev`
 * `sum`, `square`, `square_sum`

 #### `random`
 Standalone
 * `random_sample`, `pick_k`

 #### `range`
 Standalone
 * `range`
 
 #### `utility`
Standalone  
 * `make_rindex_sequence`, `make_same_sequence`, `make_zero_sequence`, `make_range_sequence`
 * `getCurrentMilliseconds`, `getCurrentMicroseconds`, `getCurrentNanoseconds`
 * `EXPECTS`, `ENSURES`, `BOUNDS_ASSERT`, `ALWAYS_ASSERT`
 
 #### `type_support`
 Requires `OpenCV`, `type_traits`, `utility`
 * `at`, `bind_at`
 * `back_emplacer`
 * `convert_to`
 * `vtype_convert`
 * `resize`
 * `add`
 * `reserved_vector`
 
