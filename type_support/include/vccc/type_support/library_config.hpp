# /*
#  * Created by YongGyu Lee on 2021/03/17.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_LIBRARY_CONFIG_HPP_
# define VCCC_TYPE_SUPPORT_LIBRARY_CONFIG_HPP_
#
# ifndef VCCC_USE_OPENCV_FEATURES
#   define VCCC_USE_OPENCV_FEATURES 0
# endif
#
# if VCCC_USE_OPENCV_FEATURES
#   include "opencv2/opencv.hpp"
# endif
#
# endif //VCCC_TYPE_SUPPORT_LIBRARY_CONFIG_HPP_
