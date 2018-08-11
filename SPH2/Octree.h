#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include "Particle.h"
#include "vec3.h"

#define NODE_LIMIT 100

using namespace std;

struct OctreeNode
{
	vector<Particle*> particles;
	OctreeNode* nodes[8];
	vec3 center;

	OctreeNode()
	{
		for (int i = 0; i < 8; i++)
			nodes[i] = NULL;
	}
};

class Octree
{

	OctreeNode* root;

	void build(OctreeNode* leaf , int depth);
	void clean(OctreeNode* leaf);
	
	
public:
	Octree()
	{
		root = NULL;
	}

	void build(vector<Particle>& particles);
};