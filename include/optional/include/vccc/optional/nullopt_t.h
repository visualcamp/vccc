# /*
#  * Created by YongGyu Lee on 2021/05/23.
#  */
#
# ifndef VCCC_OPTIONAL_NULLOPT_T_H_
# define VCCC_OPTIONAL_NULLOPT_T_H_

namespace vccc {

struct nullopt_t {
  constexpr explicit nullopt_t(int) {}
};

constexpr nullopt_t nullopt{0};

}

# endif // VCCC_OPTIONAL_NULLOPT_T_H_
