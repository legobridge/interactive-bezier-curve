#include "mesh.h"
#include "glad/glad.h"

using namespace std;

// Constructor
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
}