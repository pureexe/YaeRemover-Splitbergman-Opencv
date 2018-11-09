#pragma once

#include "../stdafx.h"

Mat DomainDownSize(Mat image)
{
	int height, width;
	height = image.rows / 2;
	width = image.cols / 2;
	Mat result(height, width, image.type());
	uchar* u = (uchar*)image.data;
	uchar* w = (uchar*)result.data;
	int i = 0, j = 0;
	int topleft, topright, bottomleft, bottomright;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
				topleft = (int) u[((2 * i) * width) + (2 * j)];
				topright = (int) u[((2 * i) * width) + (2 * j + 1)];
				bottomleft = (int) u[((2 * i + 1) * width) + (2 * j)];
				bottomright = (int) u[((2 * i + 1) * width) + (2 * j + 1)];
				w[(i * width) + j] = (uchar)((topleft + topright + bottomleft + bottomright) / 4);
		}
	}

	return result;
}