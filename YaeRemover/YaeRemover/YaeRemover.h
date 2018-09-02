#pragma once
#pragma once
#include "stdafx.h"
#include "detector/YaeDetector.h"
#include "inpainter/RecursiveInpainter.h"

class YaeRemover : public GenericVideoFilter {
public:
	YaeRemover(AVSValue args, IScriptEnvironment* env);
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
	Mat frameToMat(PVideoFrame &frame);
	PVideoFrame MatToFrame(Mat &frameArray, IScriptEnvironment* env);
private:
	int left, right, top, bottom; // Position where subtitle appear in frame
	int stokeWidth, stokeGap;
	int multiDepth, multiCoarse, multiMid, multiFine;
	double lambda, theta, tolerent, omega;
	Mat smallKernel, bigKernel, stokeKernel;
	int colorGap;
};

YaeRemover::YaeRemover(AVSValue args, IScriptEnvironment* env) :
	GenericVideoFilter(args[0].AsClip()) {
	if (vi.IsPlanar() || !vi.IsRGB()) {
		env->ThrowError("YaeRemover: input must be RGB24. Precede YaeRemover() with ConvertRGB24()");
	}
	else {
		this->left = args[1].IsInt() ? args[1].AsInt() : 0;
		this->right = args[2].IsInt() ? args[2].AsInt() : vi.width;
		this->top = args[3].IsInt() ? args[3].AsInt() : 0;
		this->bottom = args[4].IsInt() ? args[4].AsInt() : vi.height;
		this->stokeWidth = args[5].IsInt() ? args[5].AsInt() : 3;
		this->stokeGap = args[6].IsInt() ? args[6].AsInt() : 5;
		this->multiDepth = args[7].IsInt() ? args[7].AsInt() : 4;
		this->multiCoarse = args[8].IsInt() ? args[8].AsInt() : 10;
		this->multiMid = args[9].IsInt() ? args[9].AsInt() : 3;
		this->multiFine = args[10].IsInt() ? args[10].AsInt() : 10;
		this->lambda = args[11].IsFloat() ? args[11].AsFloat() : 250.0;
		this->theta = args[12].IsFloat() ? args[12].AsFloat() : 5.0;
		this->tolerent = args[13].IsFloat() ? args[13].AsFloat() : 5 * 1e-4;
		this->omega = args[14].IsFloat() ? args[14].AsFloat() : 1.0;
		this->colorGap = args[15].IsFloat() ? args[15].AsFloat() : 30;
		int smallWidth = stokeWidth - stokeGap > 0 ? (stokeWidth - stokeGap)*2 + 1: 3;
		int bigWidth = (stokeWidth + stokeGap) * 2 + 1;
		this->smallKernel = getStructuringElement(MORPH_ELLIPSE,Size(smallWidth,smallWidth));		
		this->bigKernel = getStructuringElement(MORPH_ELLIPSE, Size(bigWidth, bigWidth));
		this->stokeKernel = getStructuringElement(MORPH_ELLIPSE, Size(stokeWidth * 2 + 1, stokeWidth * 2 + 1));
	}
}

PVideoFrame __stdcall YaeRemover::GetFrame(int n, IScriptEnvironment* env) {
	PVideoFrame src = child->GetFrame(n, env);
	Mat frame = frameToMat(src);
	Rect subtitlePosition = Rect(left, frame.rows - this->bottom, this->right - this->left, this->bottom - this->top);
	Mat subtitleFrame = frame(subtitlePosition);
	Mat blackMask;
	inRange(subtitleFrame, Scalar(0, 0, 0), Scalar(colorGap, colorGap, colorGap), blackMask);
	
	
	if (countNonZero(blackMask) == 0) {
		return src; // immediately return because no-color in range
	}
	
	Mat inpainedMask = YaeDetector(blackMask,bigKernel,smallKernel);
	dilate(inpainedMask, inpainedMask, this->stokeKernel); //ขั้นที่ 8: stoke ให้เข้มขึ้นเพื่อเป็น mask	

	if (countNonZero(inpainedMask) == 0) {
		return src; // no-inpaint domain detect 
	}
	
	// rearrage image [0-255] to [0-1]
	Mat toInpaints[3], results[3];
	subtitleFrame.convertTo(subtitleFrame, CV_64FC3, 1 / 255.0);
	split(subtitleFrame, toInpaints);
	
	// inpaint
	for (int i = 0; i < 3; i++) {
		results[i] = RecusriveInpainter(toInpaints[i], inpainedMask, this->lambda, this->theta, this->omega, this->tolerent, this->multiCoarse, this->multiMid, this->multiFine, this->multiDepth, 1);
	}
	
	// rearrage image [0-1] to [0-255]
	Mat inpaintedArea;
	merge(results, 3, inpaintedArea);
	inpaintedArea.convertTo(inpaintedArea, CV_8UC3, 255.0);

	inpaintedArea.copyTo(frame(subtitlePosition));
	return MatToFrame(frame, env);
}

Mat YaeRemover::frameToMat(PVideoFrame &frame) {
	const int width = frame->GetRowSize() / 3;
	const int height = frame->GetHeight();
	BYTE* frameData = (BYTE*)frame->GetReadPtr();
	Mat mat = Mat(height, width, CV_8UC3, frameData);
	return mat.clone();
}

PVideoFrame YaeRemover::MatToFrame(Mat &mat, IScriptEnvironment* env) {
	PVideoFrame frame = env->NewVideoFrame(vi);
	BYTE* raw_data = frame->GetWritePtr();
	int x, y, n = 0, u = 0;
	const int channel_length = mat.channels();
	const int rows = mat.rows;
	const int cols = mat.cols;
	const int count = rows * cols * channel_length;
	if (channel_length == 1) {
		//we use psudo-gray scale to RGB for debug
		while (n < count) {
			raw_data[u++] = (BYTE)mat.data[n];
			raw_data[u++] = (BYTE)mat.data[n];
			raw_data[u++] = (BYTE)mat.data[n];
			n++;
		}
	}
	else {
		memcpy(raw_data, mat.data, count); //use memcpy faster than step iterate
	}
	raw_data = mat.data;
	return frame;
}

//std::wstring str = L"REMOVE SUBTITLE: BEFORE frameToArray";OutputDebugString(str.data());
