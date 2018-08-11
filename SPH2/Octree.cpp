#include "stdafx.h"
#include "Octree.h"
#include <algorithm>


void Octree::build(OctreeNode* leaf, int depth)
{
	if (leaf->particles.size() <= NODE_LIMIT) {
		
		for (int i = 0; i < leaf->particles.size(); i++)
			for (int j = 0; j < leaf->particles.size(); j++)
				if (i != j) {
					double distance = (leaf->particles[i]->position - leaf->particles[j]->position).getMagnitude();
					if (distance <= leaf->particles[i]->smoothingLength) {
						if (std::find(leaf->particles[i]->neighbors.begin(), leaf->particles[i]->neighbors.end(), leaf->particles[j] ) == leaf->particles[i]->neighbors.end())
						leaf->particles[i]->neighbors.push_back(leaf->particles[j]);
					}

				}
		
		return;
	}
//cout << depth << endl;
	vec3 center(0, 0, 0);
	for (int i = 0; i < leaf->particles.size(); i++) {
		center += leaf->particles[i]->position;
	}
	
	center /= leaf->particles.size();
	leaf->center = center;

	for (int i = 0; i < 8; i++)
		leaf->nodes[i] = new OctreeNode();

	for (int i = 0; i < leaf->particles.size(); i++)
	{
		double radius = leaf->particles[i]->smoothingLength;
		radius *= 1;
		vec3 position = leaf->particles[i]->position;
		if (position.x - radius <= center.x && position.y - radius <= center.y && position.z - radius <= center.z)
		{
			leaf->nodes[0]->particles.push_back(leaf->particles[i]);
			//cout << "0 " << position.x << " " << position.y << " " << position.z << endl;
		}
	 if (position.x - radius <= center.x && position.y - radius <= center.y && position.z >= center.z - radius)
		{
			leaf->nodes[1]->particles.push_back(leaf->particles[i]);
			//cout << "0 " << position.x << " " << position.y << " " << position.z << endl;
		}
		 if (position.x - radius <= center.x && position.y >= center.y - radius && position.z - radius <= center.z)
		{

			leaf->nodes[2]->particles.push_back(leaf->particles[i]);
			//cout << "0 " << position.x << " " << position.y << " " << position.z << endl;
		}
		 if (position.x - radius <= center.x && position.y >= center.y - radius && position.z >= center.z - radius)
		{

			leaf->nodes[3]->particles.push_back(leaf->particles[i]);
			//cout << "0 " << position.x << " " << position.y << " " << position.z << endl;
		}
		 if (position.x >= center.x - radius && position.y - radius <= center.y && position.z - radius <= center.z)
		{

			leaf->nodes[4]->particles.push_back(leaf->particles[i]);
			//cout << "0 " << position.x << " " << position.y << " " << position.z << endl;
		}
		 if (position.x >= center.x - radius && position.y - radius <= center.y && position.z >= center.z - radius)
		{

			leaf->nodes[5]->particles.push_back(leaf->particles[i]);
			//cout << "0 " << position.x << " " << position.y << " " << position.z << endl;
		}
		 if (position.x >= center.x - radius && position.y >= center.y - radius && position.z - radius <= center.z)
		{

			leaf->nodes[6]->particles.push_back(leaf->particles[i]);
			//cout << "0 " << position.x << " " << position.y << " " << position.z << endl;
		}
		 if (position.x >= center.x - radius && position.y >= center.y - radius && position.z >= center.z - radius)
		{

			leaf->nodes[7]->particles.push_back(leaf->particles[i]);
			//cout << "0 " << position.x << " " << position.y << " " << position.z << endl;
		}

	}

	for (int i = 0; i < 8; i++)
		build(leaf->nodes[i] , depth+1);
}

void Octree::build(vector<Particle>& particles)
{
	clean(root);
	root = NULL;
	root = new OctreeNode();
	vector<Particle*> particlesPtrs;
	for (int i = 0; i < particles.size(); i++)
		particlesPtrs.push_back(&particles[i]);
	root->particles = particlesPtrs;
	build(root , 0);

}

void Octree::clean(OctreeNode* leaf)
{
	if (leaf == NULL)
		return;
	for (int i = 0; i < 8; i++) {
		clean(leaf->nodes[i]);
	}
	delete leaf;
}