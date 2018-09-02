#pragma once
#include "../stdafx.h"

Mat signnum(Mat src){
	Mat z = Mat::zeros(src.size(), src.type());
	Mat a = (z < src) & 1;
	Mat b = (src < z) & 1;
	Mat dst;
	addWeighted(a, 1.0, b, -1.0, 0.0, dst, CV_64F);
	return dst;
}