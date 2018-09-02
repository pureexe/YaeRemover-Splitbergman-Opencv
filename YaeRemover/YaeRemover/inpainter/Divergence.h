#pragma once
#include "../stdafx.h"
Mat Divergence(Mat T) {
	//data extraction part
	int total_height = T.rows;
	int width = T.cols;
	int height = total_height / 2;
	Mat T1 = Mat(Size(width, height), CV_64F);
	Mat T2 = Mat(Size(width, height), CV_64F);
	Mat zeroImage = Mat::zeros(Size(width, height), CV_64F);
	T(Rect(0, 0, width, height)).copyTo(T1);
	T(Rect(0, height, width, height)).copyTo(T2);
	//discrete difference part
	Mat T1_previous = zeroImage.clone();
	T1(Rect(0, 0, width, height - 1)).copyTo(T1_previous(Rect(0, 1, width, height - 1)));
	T1 = T1 - T1_previous;
	Mat T2_previous = zeroImage.clone();
	T2(Rect(0, 0, width - 1, height)).copyTo(T2_previous(Rect(1, 0, width - 1, height)));
	T2 = T2 - T2_previous;
	return T1 + T2;
}