//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCPP_TYPE_TRAITS_MACRO_HPP
#define VCPP_TYPE_TRAITS_MACRO_HPP


#define VCPP_REQUIRE(Cond) std::enable_if_t<Cond, int> = 0


#define VCPP_REQUIRE_FWD(Cond) VCPP_REQUIRE(Cond)
#define VCPP_REQUIRE_IMPL(Cond) std::enable_if_t<Cond, int>


#endif //VCPP_TYPE_TRAITS_MACRO_HPP
