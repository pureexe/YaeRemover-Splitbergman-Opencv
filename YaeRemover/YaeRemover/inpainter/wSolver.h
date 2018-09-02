#pragma once
#include "../stdafx.h"
#include "signnum.h"

//w = max(abs(gradU + b) - (1 / theta), 0) * signnum(gradU+b);

Mat wSolver(Mat u,Mat b,double theta) {
	Mat gradU = Gradient(u);
	Mat gradB = gradU + b;
	Mat w;
	w = max(abs(gradB) - (1 / theta), 0);
	return w.mul(signnum(gradB));
}

