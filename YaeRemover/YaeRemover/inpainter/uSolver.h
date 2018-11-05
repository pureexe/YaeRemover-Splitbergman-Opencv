#pragma once
#include "../stdafx.h"
#include "Divergence.h"
Mat uSolver(Mat u, Mat w, Mat original_image, Mat b, Mat lambda, double theta, double delta, int gaussSeidelIteration) {
	Mat lapacian;
	Mat divergence = Divergence(w - b);
	Mat g;
	int i;
	
	for (i = 0; i < gaussSeidelIteration; i++) {
		Laplacian(u, lapacian, -1);
		g = lambda.mul(original_image) - divergence.mul(theta) +  lapacian.mul(theta);
		u = (u + g.mul(delta)) / (1 + lambda.mul(delta));
	}

	return u;
}