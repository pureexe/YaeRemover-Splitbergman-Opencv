#pragma once

#include "../stdafx.h"

Mat ImageDownSize(Mat image)
{
	int height, width,channel;
	height = image.rows / 2;
	width = image.cols / 2;
	channel = image.channels();
	Mat result(height, width, image.type());
	double* u = (double*) image.data;
	double* w = (double*) result.data;
	int i = 0, j = 0, k = 0;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			for (k = 0; k < channel; k++) {
				w[(i * width * channel) + (j * channel) + k] = (
					u[((2 * i) * width * channel) + (2 * j * channel) + k]
					+ u[((2 * i) * width * channel) + ((2 * j + 1) * channel) + k]
					+ u[((2 * i + 1) * width * channel) + ((2 * j) * channel) + k]
					+ u[((2 * i + 1) * width * channel) + ((2 * j + 1) * channel) + k]
				) / 4;				
			}
		}
	}

	return result;
}