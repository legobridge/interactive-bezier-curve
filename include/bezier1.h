#ifndef BEZIER1_H
#define BEZIER1_H

#include <vector>
#include <glm\glm.hpp>

namespace bezier
{
	glm::vec3 lerp(float t, glm::vec3 a, glm::vec3 b);

	std::vector<glm::vec3> iterate(float t, std::vector<glm::vec3> points);

	std::vector<glm::vec3> bezier(std::vector<glm::vec3> points, float delta_t = 0.05f);
}

#endif