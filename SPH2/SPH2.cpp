/* This code was written by Freddy Demiane */

/* Important Notice: The dimensions are all in m and kg and seconds*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "Particle.h"
#include "vec3.h"
#include "SPHMath.h"
#include "Octree.h"
#include <windows.h>

#define H 0.1
#define INITIAL_DENSITY 1000
#define MASS_MULTIPLIER 1000
#define PRESSURE_MULTIPLIER 7500
#define VISCOCITY_MULTIPLIER 0.015
#define DAMP 0.75


using namespace std;

Octree octree;

void GenerateInitialParticles(vector<Particle>& particles, vec3 dimen, vec3 sparsity);
void FindNeighboors(vector<Particle>& particles, double threshold);
void FindNeighboorsFast(vector<Particle>& particles, double threshold);
void CalculateDensity(vector<Particle>& particles, double h);
void CalculateFlux(vector<Particle>& particles, double h);
void CalculateVelocities(vector<Particle>& particles, double dt, bool firstStep);
void CalculateSurfaceTension(vector<Particle>& particles, vec3 boundaries);


int main()
{
//C:\Users\fredd\Desktop\DEMO
	ofstream file;
	file.open("C:/Users/fredd/Desktop/DEMO/output.txt");
	vector<Particle> particles;
	GenerateInitialParticles(particles , vec3(2,2,2) , vec3(20,20,20));
	bool firstStep = true;
	for (int i = 0; i < 4000; i++) 
	{
		
		cout << i << endl;
		FindNeighboorsFast(particles, H);
		CalculateDensity(particles, H);
		CalculateFlux(particles, H);
		if (i<1500)
			for (int j = 0; j < particles.size(); j++)
					particles[j].acceleration += vec3(0, 30, -20);
			
		
		CalculateVelocities(particles, 0.0025, firstStep);
		firstStep = false;
		CalculateSurfaceTension(particles, vec3(2, 2,3));
		if(i%12==0)
		for(int j = 0 ; j<particles.size() ; j++)
			file << j << " " << particles[j].position.x << " " << particles[j].position.y << " " << particles[j].position.z << endl;
		
		
		
	}

	
    return 0;
}

void GenerateInitialParticles(vector<Particle>& particles, vec3 dimen, vec3 sparsity)
{
	vec3 gap(dimen.x / sparsity.x, dimen.y / sparsity.y, dimen.z / sparsity.z);
	
	double numberOfParticles = sparsity.x*sparsity.y*sparsity.z;
	double mass = dimen.x*dimen.y*dimen.z * MASS_MULTIPLIER;
	double particleMass =  mass/numberOfParticles;
	for (int i = 0; i < sparsity.x; i++)
		for (int j = 0; j <sparsity.y; j++)
			for (int k = 0; k < sparsity.z; k++) {
				particles.push_back(Particle(vec3(i*gap.x + 0.5*gap.x  , j*gap.y + 0.5*gap.y, k*gap.z + 0.5*gap.z), particleMass, INITIAL_DENSITY, vec3(0, 0, 0)+vec3(0,0, k*gap.z + 0.5*gap.z) , H));
			}
	

}

void FindNeighboors(vector<Particle>& particles, double threshold)
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].neighbors.clear();
		for (int j = 0; j < particles.size(); j++)
		{
			
			if (i == j)
				continue;
		
			double distance = ((vec3)(particles[i].position - particles[j].position)).getMagnitude();
			if (distance <= threshold) {
				particles[i].neighbors.push_back(&particles[j]);
			}
		}
		
	}
}

void FindNeighboorsFast(vector<Particle>& particles, double threshold)
{
	for (int i = 0; i < particles.size(); i++)
		particles[i].neighbors.clear();
	octree.build(particles);
}

void CalculateDensity(vector<Particle>& particles, double h)
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].density = INITIAL_DENSITY;
		for (int j = 0; j < particles[i].neighbors.size(); j++)
		{
			Particle* pj = particles[i].neighbors[j];
			particles[i].density += pj->mass * Poly6Kernel(particles[i].position, pj->position, h);
			
			
		}

		particles[i].pressure = PRESSURE_MULTIPLIER * (particles[i].density - INITIAL_DENSITY);
		
	}
}

void CalculateDensity2(vector<Particle>& particles, double h)
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].density = 4*particles[i].mass / (PI*h*h);
		double sum = 0;
		for (int j = 0; j < particles[i].neighbors.size(); j++)
		{
			Particle* pj = particles[i].neighbors[j];
			//particles[i].density += pj->mass * Poly6Kernel(particles[i].position, pj->position, h);
			double r = (particles[i].position - pj->position).getMagnitude();
			sum += pow((h*h - r * r), 3);


		}
		particles[i].density += (4 * particles[i].mass / (pow(h, 8)*PI))*(sum);


	}
}

void CalculateFlux2(vector<Particle>&particles, double h)
{
	double k = 10;
	double mu = 0.1;
	vec3 sum = vec3(0, 0, 0);
	for (int i = 0; i < particles.size(); i++)
	{
		Particle* pi = &particles[i];
		double pri = pi->density;
		for (int j = 0; j < particles[i].neighbors.size(); j++)
		{
			Particle* pj = particles[i].neighbors[j];
			double prj = pj->density;
			double qij = (pi->position - pj->position).getMagnitude()/h;

			//

			double f1 = pj->mass / (PI*h*h*h*h*prj);
			double f2 = 1 - qij;
			double f3 = 15 * k * (pri + prj - 2 * INITIAL_DENSITY);
			double f4 = (1 - qij) / qij;
			vec3 f5 = pi->position - pj->position;
			vec3 ff = f5 * (f3*f4);
			vec3 ff2 = (pi->velocity - pj->velocity) * 40*mu;
			vec3 f = ff - ff2;

			vec3 interact = f * (f1*f2);
			sum += interact ;

			
		}

		particles[i].acceleration = (sum / pri)+vec3(0,-10,0);
		vec3 test = (sum / pri);
		//cout << test.x << " " << test.y << " " << test.z << endl;
	}
	//system("pause");






}

void CalculateFlux(vector<Particle>& particles, double h)
{
	for (int i = 0; i < particles.size(); i++)
	{
		Particle* pi = &particles[i];
		vec3 pForce = vec3(0, 0, 0);
		vec3 vForce = vec3(0, 0, 0);
		vec3 gForce = vec3(0, -10, 0);

		for (int j = 0; j < particles[i].neighbors.size(); j++)
		{
			Particle* pj = particles[i].neighbors[j];

			double f1 = pj->mass * pi->pressure / (pi->density*pi->density);
			vec3 f2 = SpikyGradient(pi->position, pj->position, h);
			vec3 pressureForce = f2 * f1;

			vec3 f3 = (pj->velocity - pi->velocity)* pj->mass;
			double f4 = PolyLaplacian(pi->position, pj->position, h);
			vec3 viscocityForce = f3 * f4;

			
			pForce += pressureForce;
			vForce += viscocityForce;
		}

		vForce *= (double) (VISCOCITY_MULTIPLIER / pi->density);
		
	
		particles[i].sumOfForces = gForce + pForce + vForce;
		particles[i].acceleration = particles[i].sumOfForces;
		
	}
}

void CalculateVelocities(vector<Particle>& particles, double dt, bool firstStep)
{
	for (int i = 0; i < particles.size(); i++)
	{
		if (firstStep)
		{
			particles[i].halfVelocity = particles[i].velocity + particles[i].acceleration*0.5*dt;
			particles[i].velocity += particles[i].acceleration*dt;
			particles[i].position += particles[i].halfVelocity*dt;
		}
		else
		{
			particles[i].halfVelocity += particles[i].acceleration*dt;
			particles[i].velocity = particles[i].halfVelocity + particles[i].acceleration*0.5*dt;
			particles[i].position+= particles[i].halfVelocity*dt;
		}
	}
}

void CalculateSurfaceTension(vector<Particle>& particles, vec3 boundaries)
{
	

	for (int i = 0; i < particles.size(); i++)
	{


		// From Bindell
		
		if (particles[i].position.y <= -boundaries.y)
		{
			float tbounce = (particles[i].position.y + boundaries.y) / particles[i].velocity.y;
			particles[i].position -= particles[i].velocity * (1 - DAMP)*tbounce;

			particles[i].position.y = 2 * (-boundaries.y) - particles[i].position.y;
			particles[i].velocity.y = -particles[i].velocity.y;
			particles[i].halfVelocity.y = -particles[i].halfVelocity.y;

			particles[i].velocity = particles[i].velocity* DAMP;
			particles[i].halfVelocity = particles[i].halfVelocity* DAMP;


		}

		if (particles[i].position.y >= boundaries.y)
		{
			float tbounce = (particles[i].position.y - boundaries.y) / particles[i].velocity.y;
			particles[i].position -= particles[i].velocity * (1 - DAMP)*tbounce;

			particles[i].position.y = 2 * (boundaries.y) - particles[i].position.y;
			particles[i].velocity.y = -particles[i].velocity.y;
			particles[i].halfVelocity.y = -particles[i].halfVelocity.y;

			particles[i].velocity = particles[i].velocity* DAMP;
			particles[i].halfVelocity = particles[i].halfVelocity* DAMP;


		}

		if (particles[i].position.x <= -boundaries.x*0.1)
		{
			float tbounce = (particles[i].position.x + boundaries.x*0.1) / particles[i].velocity.x;
			particles[i].position -= particles[i].velocity * (1 - DAMP)*tbounce;

			particles[i].position.x = 2 * (-boundaries.x*0.1) - particles[i].position.x;
			particles[i].velocity.x = -particles[i].velocity.x;
			particles[i].halfVelocity.x = -particles[i].halfVelocity.x;

			particles[i].velocity = particles[i].velocity* DAMP;
			particles[i].halfVelocity = particles[i].halfVelocity* DAMP;


		}

		if (particles[i].position.x >= boundaries.x)
		{
			float tbounce = (particles[i].position.x - boundaries.x) / particles[i].velocity.x;
			particles[i].position -= particles[i].velocity * (1 - DAMP)*tbounce;

			particles[i].position.x = 2 * (boundaries.x) - particles[i].position.x;
			particles[i].velocity.x = -particles[i].velocity.x;
			particles[i].halfVelocity.x = -particles[i].halfVelocity.x;

			particles[i].velocity = particles[i].velocity* DAMP;
			particles[i].halfVelocity = particles[i].halfVelocity* DAMP;


		}

		if (particles[i].position.z <= -boundaries.z)
		{
			float tbounce = (particles[i].position.z + boundaries.z) / particles[i].velocity.z;
			particles[i].position -= particles[i].velocity * (1 - DAMP)*tbounce;

			particles[i].position.z = 2 * (-boundaries.z) - particles[i].position.z;
			particles[i].velocity.z = -particles[i].velocity.z;
			particles[i].halfVelocity.z = -particles[i].halfVelocity.z;

			particles[i].velocity = particles[i].velocity* DAMP;
			particles[i].halfVelocity = particles[i].halfVelocity* DAMP;


		}

		if (particles[i].position.z >= boundaries.z)
		{
			float tbounce = (particles[i].position.z - boundaries.z) / particles[i].velocity.z;
			particles[i].position -= particles[i].velocity * (1 - DAMP)*tbounce;

			particles[i].position.z = 2 * (boundaries.z) - particles[i].position.z;
			particles[i].velocity.z = -particles[i].velocity.z;
			particles[i].halfVelocity.z = -particles[i].halfVelocity.z;

			particles[i].velocity = particles[i].velocity* DAMP;
			particles[i].halfVelocity = particles[i].halfVelocity* DAMP;


		}


	}

}