//
// Created by YongGyu Lee on 3/22/24.
//

#ifndef VCCC_LOG_GLOBAL_STREAM_WRAPPER_SETTINGS_HPP_
#define VCCC_LOG_GLOBAL_STREAM_WRAPPER_SETTINGS_HPP_

namespace vccc {

/// @addtogroup log
/// @{

class GlobalStreamWrapperSettings {
 public:
  GlobalStreamWrapperSettings(const GlobalStreamWrapperSettings&) = delete;
  GlobalStreamWrapperSettings(GlobalStreamWrapperSettings&&) = delete;
  GlobalStreamWrapperSettings& operator=(const GlobalStreamWrapperSettings&) = delete;
  GlobalStreamWrapperSettings& operator=(GlobalStreamWrapperSettings&&) = delete;

  // Add quotation to strings (string and string_view)
  static bool quote_string() {
    return GetInstance().quote_string_;
  }

  static void quote_string(bool new_value) {
    GetInstance().quote_string_ = new_value;
  }

  // Expand aggregate types (non-empty aggregate types)
  static bool expand_aggregate() {
    return GetInstance().expand_aggregate_;
  }

  static void expand_aggregate(bool new_value) {
    GetInstance().expand_aggregate_ = new_value;
  }

 private:
  GlobalStreamWrapperSettings() = default;

  static GlobalStreamWrapperSettings& GetInstance() {
    static auto inst = new GlobalStreamWrapperSettings();
    return *inst;
  }

  bool quote_string_ = false;
  bool expand_aggregate_ = false;
};

/// @}

} // namespace vccc

#endif // VCCC_LOG_GLOBAL_STREAM_WRAPPER_SETTINGS_HPP_
