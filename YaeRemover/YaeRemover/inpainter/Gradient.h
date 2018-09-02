#pragma once
#include "../stdafx.h"


Mat Gradient(Mat u) {
	double h = 1;
	int height = u.rows;
	int width = u.cols;
	Mat grad = Mat::zeros(Size(width, height * 2), CV_64F);
	Mat u_left = u.clone();
	Mat u_up = u.clone();
	u(Rect(1, 0, width-1,height)).copyTo(u_left(Rect(0, 0, width - 1, height)));
	u_left = u_left - u;
	u(Rect(0, 1, width, height - 1)).copyTo(u_up(Rect(0, 0, width, height - 1)));
	u_up = u_up - u;
	u_up.copyTo(grad(Rect(0,0,width,height)));
	u_left.copyTo(grad(Rect(0, height, width, height)));
	return grad;
}