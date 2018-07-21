#pragma once
#include <math.h>
#include "vec3.h"
#define PI 3.14159265359

double CubicKernel(vec3 r, vec3 rb, double h);
vec3 CubicKernelGradient(vec3 r, vec3 rb, double h);
double CubicKernelLaplacian(vec3 r, vec3 rb, double h);
double Poly6Kernel(vec3 r, vec3 rb, double h);
vec3 SpikyGradient(vec3 r, vec3 rb, double h);
double PolyLaplacian(vec3 r, vec3 rb, double h);
