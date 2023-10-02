# /*
#  * Created by YongGyu Lee on 2021/05/23.
#  */
#
# ifndef VCCC_OPTIONAL_INPLACE_T_H_
# define VCCC_OPTIONAL_INPLACE_T_H_

namespace vccc {

struct in_place_t {
  constexpr explicit in_place_t() = default;
};

constexpr in_place_t in_place{};

}

# endif // VCCC_OPTIONAL_INPLACE_T_H_
