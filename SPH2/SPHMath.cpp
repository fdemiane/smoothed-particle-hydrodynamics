#include "stdafx.h"
#include "SPHMath.h"

double CubicKernel(vec3 r, vec3 rb, double h)
{
	//if ((r - rb).getMagnitude() > h)
		//return 0;
	double f1 = 315.0 / (64.0*PI * pow(h, 9));
	double f2 = h * h - pow((r - rb).getMagnitude(), 2);
	return f1 * f2*f2*f2;
}

vec3 CubicKernelGradient(vec3 r, vec3 rb, double h)
{
	//if ((r - rb).getMagnitude() > h)
	//	return vec3(0.0, 0.0, 0.0);
	double f1 = 45.0 / (PI*pow(h, 6));
	double f2 = h - (r - rb).getMagnitude();
	f2 = f2 * f2;
	vec3 diff = r - rb;
	double l = diff.getMagnitude();

	return vec3(f1*f2*(diff.x) / l, f1*f2*(diff.y) / l, f1*f2*(diff.z) / l);
}

double CubicKernelLaplacian(vec3 r, vec3 rb, double h)
{
	//if ((r - rb).getMagnitude() > h)
		//return 0;
	double f1 = 45.0 / (PI*pow(h, 6));
	double f2 = h - (r - rb).getMagnitude();
	return f1 * f2;
}

double Poly6Kernel(vec3 r, vec3 rb, double h)
{
	//if ((r - rb).getMagnitude() > h)
		//return 0;
	double radius = (r - rb).getMagnitude();
	double f1 = 315.0/(64.0 * PI*pow(h, 9));
	double f2 = pow(h*h - radius*radius, 3);
	return f1 * f2;
}

vec3 SpikyGradient(vec3 r, vec3 rb, double h)
{

	//if ((r - rb).getMagnitude() > h)
		//return vec3(0.0, 0.0, 0.0);
	double radius = (r - rb).getMagnitude();
	double f1 = -45.0 / (PI * pow(h, 6));
	double f2 = pow(h - radius, 2);
	double f3 = f1 * f2;
	return (rb - r)*f3; // sign change!
}

double PolyLaplacian(vec3 r, vec3 rb, double h)
{
	//if ((r - rb).getMagnitude() > h)
		//return 0;
	double radius = (r - rb).getMagnitude();
	double f1 = 45.0 / (PI * pow(h, 6));
	double f2 = h - radius;
	return f1 * f2;
}

