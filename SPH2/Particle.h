#pragma once
#include "vec3.h"
#include <vector>

using namespace std;

struct Particle
{
	vector<Particle*> neighbors;

	double mass;
	double density;
	double pressure;

	vec3 sumOfForces;

	vec3 acceleration;
	vec3 velocity;
	vec3 position;

	vec3 halfVelocity;

	

	Particle()
	{
		velocity.x = 0;
		velocity.y = 0;
		velocity.z = 0;

		acceleration.x = 0;
		acceleration.y = 0;
		acceleration.z = 0;

		halfVelocity.x = 0;
		halfVelocity.y = 0;
		halfVelocity.z = 0;
	}

	Particle(vec3 initialPosition, double m, double d)
	{
		position.x = initialPosition.x;
		position.y = initialPosition.y;
		position.z = initialPosition.z;

		velocity.x = 0;
		velocity.y = 0;
		velocity.z = 0;

		acceleration.x = 0;
		acceleration.y = 0;
		acceleration.z = 0;

		halfVelocity.x = 0;
		halfVelocity.y = 0;
		halfVelocity.z = 0;
		
		sumOfForces = vec3(0, 0, 0);

		mass = m;
		density = d;
	}

	Particle(vec3 initialPosition, double m, double d , vec3 vel)
	{
		position.x = initialPosition.x;
		position.y = initialPosition.y;
		position.z = initialPosition.z;

		velocity = vel;

		acceleration.x = 0;
		acceleration.y = 0;
		acceleration.z = 0;

		halfVelocity.x = 0;
		halfVelocity.y = 0;
		halfVelocity.z = 0;

		sumOfForces = vec3(0, 0, 0);

		mass = m;
		density = d;
	}
};
