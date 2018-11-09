#pragma once

#include "../stdafx.h"

#include "SplitBergmanSolver.h"
#include "MergeResult.h"
#include "ImageDownSize.h"
#include "DomainDownSIze.h"

Mat RecusriveInpainter(Mat original_image, Mat inpainted_domain, double lambda, double theta, double delta, int gaussSeidelIteration, double tolerant, int iteration_coarse, int iteration_mid, int iteration_fine, int max_step, int current_step)
{
	std::wstring str;
	Mat toinpaint_image;
	int max_iteration = iteration_fine;
	if (current_step < max_step) {
		int height = original_image.rows;
		int width = original_image.cols;
		Mat half_image, half_domain, half_result, upscale_result;
		resize(original_image, half_image, cv::Size(width * 0.5, height * 0.5));
		resize(inpainted_domain, half_domain, cv::Size(width * 0.5, height * 0.5));
		half_result = RecusriveInpainter(half_image, half_domain, lambda, theta, delta, gaussSeidelIteration, tolerant, iteration_coarse, iteration_mid, iteration_fine, max_step, current_step + 1);
		resize(half_result, upscale_result, cv::Size(width, height));
		toinpaint_image = MergeResult(original_image, inpainted_domain, upscale_result);
	}
	else {
		toinpaint_image = original_image;
	}
	Mat inpainted_lambda = (inpainted_domain == 0);
	inpainted_lambda.convertTo(inpainted_lambda, CV_64F, lambda/255.0);
	if (current_step != 1) {
		if (current_step == max_step) {
			max_iteration = iteration_coarse;
		}
		else {
			max_iteration = iteration_mid;
		}
	}
	return SplitBergmanSolver(toinpaint_image, inpainted_lambda, theta, delta, gaussSeidelIteration, tolerant, max_iteration);
}