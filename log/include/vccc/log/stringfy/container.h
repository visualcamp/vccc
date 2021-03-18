# /*
#  * Created by YongGyu Lee on 2021/03/12.
#  */
#
# ifndef VCCC_LOG_STRINGFY_CONTAINER_H_
# define VCCC_LOG_STRINGFY_CONTAINER_H_
#
# include "vccc/log/stringfy/forward_declare.h"
# include <chrono>

namespace vccc {

template<typename T, VCCC_ENABLE_IF_IMPL(is_container_v<T>)> std::string stringfy(const T& value) {

}

}

# endif //VCCC_LOG_STRINGFY_CONTAINER_H_
