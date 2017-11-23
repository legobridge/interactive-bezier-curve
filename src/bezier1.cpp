#include <iostream>
#include "bezier1.h"

using namespace std;

namespace bezier
{
	// Return linearly interpolated point between a and b
	glm::vec3 lerp(float t, glm::vec3 a, glm::vec3 b)
	{
		return (1 - t) * a + t * b;
	}

	// Perform the linear interpolation on each consecutive pair of points
	vector<glm::vec3> iterate(float t, vector<glm::vec3> points)
	{
		vector<glm::vec3> new_points;

		for (size_t i = 0; i + 1 < points.size(); i++)
		{
			new_points.push_back(lerp(t, points[i], points[i + 1]));
		}

		return new_points;

	}

	// Return vector of points on the computed Bezier curve
	vector<glm::vec3> bezier(vector<glm::vec3> points, float delta_t)
	{
		float t = 0;
		float times = round(1.0f / delta_t);
		vector<glm::vec3> curve;

		while (times--)
		{
			vector<glm::vec3> new_points = points;
			for (size_t i = 0; i + 1 < points.size(); i++)
			{
				new_points = iterate(t, new_points);
			}
			if (!new_points.empty())
			{
				curve.push_back(new_points[0]);
			}
			t += delta_t;
		}

		vector<glm::vec3> new_points = points;
		for (size_t i = 0; i + 1 < points.size(); i++)
		{
			new_points = iterate(1.0f, new_points);
		}

		if (!new_points.empty())
		{
			curve.push_back(new_points[0]);
		}
		return curve;
	}
}