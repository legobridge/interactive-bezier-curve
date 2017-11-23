#ifndef BEZIER1_H
#define BEZIER1_H

#include <vector>
#include <glm\glm.hpp>

namespace bezier
{
	// Return linearly interpolated point between a and b
	glm::vec3 lerp(float t, glm::vec3 a, glm::vec3 b);

	// Perform the linear interpolation on each consecutive pair of points
	std::vector<glm::vec3> iterate(float t, std::vector<glm::vec3> points);

	// Return vector of points on the computed Bezier curve
	std::vector<glm::vec3> bezier(std::vector<glm::vec3> points, float delta_t = 0.05f);
}

#endif