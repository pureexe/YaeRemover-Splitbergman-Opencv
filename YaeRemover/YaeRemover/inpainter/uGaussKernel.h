#pragma once
#include "../stdafx.h"
double uGaussKernel(double u, double z,double lambda, double theta,double omega, double divergence,
	double u_left, double u_right, double u_above, double u_under)
{
	double lapacian = u_left + u_right + u_above + u_under;
	double buffer = lambda * z - theta * divergence + theta * lapacian;
	double u_new = (1 / (lambda + 4 * theta))*buffer;
	return (1 - omega) * u + omega *u_new;
}