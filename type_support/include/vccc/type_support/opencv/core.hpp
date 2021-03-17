# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_OPENCV_CORE_HPP
# define VCCC_TYPE_SUPPORT_OPENCV_CORE_HPP
#
# include "vccc/type_support/library_config.hpp"
# include "vccc/type_support/opencv/traits.hpp"
#
# if VCCC_USE_OPENCV_FEATURES
#   include "vccc/type_support/opencv/cv_mat.hpp"
#   include "vccc/type_support/opencv/cv_point.hpp"
#   include "vccc/type_support/opencv/cv_point3.hpp"
#   include "vccc/type_support/opencv/cv_size.hpp"
#   include "vccc/type_support/opencv/cv_vec.hpp"
# endif
#
# endif //VCCC_TYPE_SUPPORT_OPENCV_CORE_HPP