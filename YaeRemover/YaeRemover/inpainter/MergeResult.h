#pragma once

#include "../stdafx.h"

Mat MergeResult(Mat original_image, Mat inpainted_domain, Mat upscale_result)
{
	Mat upscale_condition = (inpainted_domain > 0) / 255.0;
	Mat original_condition = (inpainted_domain == 0) / 255.0;
	upscale_condition.convertTo(upscale_condition, CV_64F);
	original_condition.convertTo(original_condition, CV_64F);
	return  (upscale_result.mul(upscale_condition)) + (original_image.mul(original_condition));
}