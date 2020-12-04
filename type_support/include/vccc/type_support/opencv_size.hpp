//
// Created by YongGyu Lee on 2020/12/02.
//

#ifndef VCCC_TYPE_SUPPORT_OPENCV_SIZE_HPP
#define VCCC_TYPE_SUPPORT_OPENCV_SIZE_HPP

#include "vccc/type_support/detail/cv_size.hpp"
#include "opencv2/core/types.hpp"

namespace vc{

template<typename T>
struct detail::cv_size<cv::Point_<T>> : cv_size_n<2> {};

template<typename T>
struct detail::cv_size<cv::Point3_<T>> : cv_size_n<3> {};

template<typename T, int m, int n>
struct detail::cv_size<cv::Matx<T, m, n>> : cv_size_n<m*n> {};

template<typename T, int cn>
struct detail::cv_size<cv::Vec<T, cn>> : cv_size_n<cn> {};

template<typename T>
struct detail::cv_size<cv::Size_<T>> : cv_size_n<2> {};


}

#endif //VCCC_TYPE_SUPPORT_OPENCV_SIZE_HPP
