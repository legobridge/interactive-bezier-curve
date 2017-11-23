#ifndef SURFACE_H
#define SURFACE_H

#include <vector>
#include <glm\glm.hpp>

class Surface
{
public:
	Mesh getSurface(std::vector<glm::vec3> controlPoints, float delta_a = 10.0f);
};

#endif