//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCPP_LOG_MACRO_HPP
#define VCPP_LOG_MACRO_HPP


#define PWD                                     \
[](){                                           \
  std::string $13vlaae_dummy_str = __FILE__;    \
  return $13vlaae_dummy_str.substr(             \
    0, $13vlaae_dummy_str.find_last_of('/'));   \
}()



#endif //VCPP_LOG_MACRO_HPP
