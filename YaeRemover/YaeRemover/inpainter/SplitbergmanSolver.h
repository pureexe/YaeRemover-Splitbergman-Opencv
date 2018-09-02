#pragma once
#include "../stdafx.h"
#include "Gradient.h"
#include "wSolver.h"
#include "uSolver.h"

Mat SplitBergmanSolver(Mat original_image, Mat lambda, double theta, double omega, double tolerant, int max_iteration) {
	double image_norm = 9999999;
	int i = 1;
	Mat last_image;
	Mat u = original_image.clone();
	int height = original_image.rows;
	int width = original_image.cols;
	Mat b = Mat::zeros(Size(width, height * 2), CV_64F);
	Mat w = Mat::zeros(Size(width, height * 2), CV_64F);
	while (image_norm > tolerant && i < max_iteration) {		
		last_image = u.clone();
		w = wSolver(u, b, theta);
		u = uSolver(u, w, original_image, b, lambda, theta, omega);
		b = b + Gradient(u) - w; 
		image_norm = norm(u - last_image) / norm(u);
		i++;
	}
	//printf("processed with %d loops\n", i);
	return u;
}