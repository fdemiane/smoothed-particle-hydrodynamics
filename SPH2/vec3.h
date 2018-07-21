#pragma once
#include <math.h>

using namespace std;

struct vec3
{
	double x, y, z;
	vec3()
	{
		x = y = z = 0;
	}

	vec3(double a, double b, double c)
	{
		x = a;
		y = b;
		z = c;
	}

	double getMagnitude()
	{
		return sqrt(x*x + y * y + z * z);
	}

	vec3 Normalized()
	{
		double m = getMagnitude();
		return vec3(x / m, y / m, z / m);
	}

	vec3 operator+(const vec3& other)
	{
		double tx = x + other.x;
		double ty = y + other.y;
		double tz = z + other.z;
		vec3 f = vec3(tx, ty, tz);
		return f;
	}

	vec3 operator-(const vec3& other)
	{
		double tx = x - other.x;
		double ty = y - other.y;
		double tz = z - other.z;
		vec3 f = vec3(tx, ty, tz);
		return f;
	}


	vec3 operator*(const double& other)
	{
		double tx = x * other;
		double ty = y * other;
		double tz = z * other;
		vec3 f = vec3(tx, ty, tz);
		return f;
	}

	vec3 operator/(const double& other)
	{
		return vec3(x / other, y / other, z / other);
	}

	vec3& operator+=(const vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}
	vec3& operator/=(const double& other)
	{
		x /= other;
		y /= other;
		z /= other;

		return *this;

	}

	vec3& operator-=(const vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	vec3& operator*=(const vec3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}

	vec3& operator*=(const double& other)
	{
		x *= other;
		y *= other;
		z *= other;

		return *this;
	}
};
