#ifndef MESH_H
#define MESH_H

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
};

class Mesh
{
public:

	// Mesh Data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// Constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
};
#endif