#pragma once
#include "../stdafx.h"
#include "uGaussKernel.h"
#include "Divergence.h"
Mat uSolver(Mat u, Mat w, Mat original_image, Mat b, Mat lambda, double theta, double omega) {
	Mat lapacian;
	Mat divergence = Divergence(w - b);
	Mat g;
	double delta = 1 / 5;
	int i;
	int max_gauss_seidel = 10;
	
	for (i = 0; i < max_gauss_seidel; i++) {
		Laplacian(u, lapacian, CV_64FC3);
		g = lambda * original_image - theta * divergence + theta *lapacian;
		u = (u + delta * g) / (1 + delta *lambda);
	}

	return u;
}