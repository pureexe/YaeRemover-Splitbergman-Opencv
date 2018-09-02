#pragma once
#include "../stdafx.h"
#include "uGaussKernel.h"
#include "Divergence.h"
Mat uSolver(Mat u, Mat w, Mat original_image, Mat b, Mat lambda, double theta) {
	int height = u.rows;
	int width = u.cols;
	int i, j;
	Mat divergence = Divergence(w - b);
	//top - left corner
	u.at<double>(0, 0) = uGaussKernel(
		u.at<double>(0, 0), original_image.at<double>(0, 0),
		lambda.at<double>(0, 0), theta, divergence.at<double>(0, 0),
		u.at<double>(0, 0), u.at<double>(0, 1), u.at<double>(0, 0), u.at<double>(1, 0));
	//top-bar line
	for (i = 1; i < width - 1; i++) {
		u.at<double>(0, i) = uGaussKernel(u.at<double>(0, i), original_image.at<double>(0, i),
			lambda.at<double>(0, i), theta, divergence.at<double>(0, i),
			u.at<double>(0, i - 1), u.at<double>(0, i + 1), u.at<double>(0, i), u.at<double>(1, i));
	}
	//top - right corner
	u.at<double>(0, width - 1) = uGaussKernel(u.at<double>(0, width - 1), original_image.at<double>(0, width - 1),
		lambda.at<double>(0, width - 1), theta, divergence.at<double>(0, width - 1),
		u.at<double>(0, width - 2), u.at<double>(0, width - 1), u.at<double>(0, width - 1), u.at<double>(1, width - 1));
	//body
	for (i = 1; i < height - 1; i++) {
		//body - front
		u.at<double>(i, 0) = uGaussKernel(u.at<double>(i, 0), original_image.at<double>(i, 0),
			lambda.at<double>(i, 0), theta, divergence.at<double>(i, 0),
			u.at<double>(i, 0), u.at<double>(i, 1), u.at<double>(i - 1, 0), u.at<double>(i + 1, 0));
		//body - interior
		for (j = 1; j < width - 1; j++) {
			u.at<double>(i, j) = uGaussKernel(u.at<double>(i, j), original_image.at<double>(i, j),
				lambda.at<double>(i, j), theta, divergence.at<double>(i, j),
				u.at<double>(i, j - 1), u.at<double>(i, j + 1), u.at<double>(i - 1, j), u.at<double>(i + 1, j));
		}
		//body - rear
		u.at<double>(i, width - 1) = uGaussKernel(u.at<double>(i, width - 1), original_image.at<double>(i, width - 1),
			lambda.at<double>(i, width - 1), theta, divergence.at<double>(i, width - 1),
			u.at<double>(i, width - 2), u.at<double>(i, width - 1), u.at<double>(i - 1, width - 1), u.at<double>(i + 1, width - 1));
	}
	//bottom-left
	u.at<double>(height - 1, 0) = uGaussKernel(u.at<double>(height - 1, 0), original_image.at<double>(height - 1, 0),
		lambda.at<double>(height - 1, 0), theta, divergence.at<double>(height - 1, 0),
		u.at<double>(height - 1, 0), u.at<double>(height - 1, 1), u.at<double>(height - 2, 0), u.at<double>(height - 1, 0));
	//bottom - bar line
	for (i = 1; i < width - 1; i++) {
		u.at<double>(height - 1, i) = uGaussKernel(u.at<double>(height - 1, i), original_image.at<double>(height - 1, i),
			lambda.at<double>(height - 1, i), theta, divergence.at<double>(height - 1, i),
			u.at<double>(height - 1, i - 1), u.at<double>(height - 1, i + 1), u.at<double>(height - 2, i), u.at<double>(height - 1, i));
	}
	//bottom-right
	u.at<double>(height - 1, width - 1) = uGaussKernel(u.at<double>(height - 1, width - 1), original_image.at<double>(height - 1, width - 1),
		lambda.at<double>(height - 1, width - 1), theta, divergence.at<double>(height - 1, width - 1),
		u.at<double>(height - 1, width - 2), u.at<double>(height - 1, width - 1), u.at<double>(height - 2, width - 1), u.at<double>(height - 1, width - 1));
	return u;
}