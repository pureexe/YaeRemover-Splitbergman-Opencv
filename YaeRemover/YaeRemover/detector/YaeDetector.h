#pragma once
#include "../stdafx.h"

Mat YaeDetector(Mat &blackMask,Mat &bigKernel,Mat &smallKernel) {
	Mat invertedMask,cavityMask,cavityMaskWithNoise,cavityMaskNoNoise,cleary,noisy,floodedMask,flooededCanvasMask,floodedClearyMask,floodedNoisyMask,largeObject,subtitle;
	Scalar zeroHolder = Scalar(0);
	int width = blackMask.cols;
	int height = blackMask.rows;
	int count = height * width;
	int i,x, y;
	uchar *largeObjectPtr, *cavityMaskWithNoisePtr, *clearyPtr;
	//ขั้น 2: สลับสี
	bitwise_not(blackMask, invertedMask);

	//ขั้น 3: ถมดำ
	cavityMask = invertedMask;
	floodedMask = Mat::zeros(height + 2, width + 2, CV_8U);
	
	for (y = 0; y < height; y++) {
		floodFill(cavityMask, floodedMask, Point(0, y), zeroHolder);
		floodFill(cavityMask, floodedMask, Point(width - 1, y), zeroHolder);
	}
	for (x = 0; x < width; x++) {
		floodFill(cavityMask, floodedMask, Point(x, 0), zeroHolder);
		floodFill(cavityMask, floodedMask, Point(x, height - 1), zeroHolder);
	}

	//ขั้นที่ 4: หาพื้นที่ใหญ่เกินไปในภาพ
	erode(cavityMask, largeObject, bigKernel);
	//ขั้นที่ 5: ถมวัตถุที่ใหญ่เกินไป
	flooededCanvasMask = Mat::zeros(height + 2, width + 2, CV_8U);
	cavityMaskWithNoise = cavityMask.clone();
	largeObjectPtr = largeObject.data;
	cavityMaskWithNoisePtr = cavityMaskWithNoise.data;
	for (i = 0; i < count; i++) {
		if (largeObjectPtr[i] != 0 && cavityMaskWithNoisePtr[i] != 0) {
			floodFill(cavityMaskWithNoise, floodedMask, Point(i % width, i / width), zeroHolder);
		}
	}
	//ขั้น 6: ลบพื้นที่เล็กเกินไปออกจากภาพ
	morphologyEx(cavityMaskWithNoise, cavityMaskNoNoise, MORPH_OPEN, smallKernel);
	//ขั้นที่ 7:ใช้ connected component labeling เพื่อเอาส่วนที่ต้องการโดยไม่ให้ตัวอักษรเสียหายมากนัก	
	cleary = cavityMaskNoNoise;
	noisy = cavityMaskWithNoise.clone();
	clearyPtr = cleary.data;
	for (i = 0; i < count; i++) {
		if (clearyPtr[i] != 0) {
			floodFill(cleary, floodedClearyMask, Point(i % width, i / width), zeroHolder);
			floodFill(noisy, floodedNoisyMask, Point(i % width, i / width), zeroHolder);
		}
	}
	bitwise_xor(cavityMaskWithNoise, noisy, subtitle);
	return subtitle;
}