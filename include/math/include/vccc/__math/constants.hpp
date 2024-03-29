# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
#ifndef VCCC_MATH_CONSTANTS_HPP
#define VCCC_MATH_CONSTANTS_HPP

namespace vccc {
namespace math {

/**
@addtogroup math
@{
    @defgroup math_constants__var (constants)
@{
*/
//#define M_E         2.71828182845904523536028747135266250   /* e              */
//#define M_LOG2E     1.44269504088896340735992468100189214   /* log2(e)        */
//#define M_LOG10E    0.434294481903251827651128918916605082  /* log10(e)       */
//#define M_LN2       0.693147180559945309417232121458176568  /* loge(2)        */
//#define M_LN10      2.30258509299404568401799145468436421   /* loge(10)       */
//#define M_PI        3.14159265358979323846264338327950288   /* pi             */
//#define M_PI_2      1.57079632679489661923132169163975144   /* pi/2           */
//#define M_PI_4      0.785398163397448309615660845819875721  /* pi/4           */
//#define M_1_PI      0.318309886183790671537767526745028724  /* 1/pi           */
//#define M_2_PI      0.636619772367581343075535053490057448  /* 2/pi           */
//#define M_2_SQRTPI  1.12837916709551257389615890312154517   /* 2/sqrt(pi)     */
//#define M_SQRT2     1.41421356237309504880168872420969808   /* sqrt(2)        */
//#define M_SQRT1_2   0.707106781186547524400844362104849039  /* 1/sqrt(2)      */
//
//#define MAXFLOAT    0x1.fffffep+127f

/** @brief the mathematical constant \f$e\f$
 * @tparam T
 */
template<typename T>
constexpr T e = static_cast<T>(2.71828182845904523536028747135266250);

//template<typename T>
//constexpr auto log2e = 1.44269504088896340735992468100189214;
//
//template<typename T>
//constexpr auto log10e =


/** @brief the mathematical constant \f$\pi\f$
 * @tparam T
 */
template<typename T>
constexpr T pi = static_cast<T>(3.14159265358979323846264338327950288);


//! @}
//! @}

} // namespace math
} // namespace vccc

#endif // VCCC_MATH_CONSTANTS_HPP
