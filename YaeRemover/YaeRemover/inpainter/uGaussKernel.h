#pragma once
#include "../stdafx.h"
double uGaussKernel(double u, double z,double lambda, double theta, double divergence,
	double u_left, double u_right, double u_above, double u_under)
{
	double h = 1;
	double lapacian = (1/h*h)*(u_left + u_right + u_above + u_under);
	double buffer = lambda * z - theta * divergence + theta * lapacian;
	return ((h*h) / (lambda*(h*h) + 4 * theta))*buffer;
}