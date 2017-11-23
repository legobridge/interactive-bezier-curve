#ifndef SURFACE_H
#define SURFACE_H

#include <vector>
#include <glm\glm.hpp>

class Surface
{
public:
	// Return a mesh that defines a surface of revolution about the Y-axis
	Mesh getSurface(std::vector<glm::vec3> controlPoints, float delta_a = 10.0f);
};

#endif