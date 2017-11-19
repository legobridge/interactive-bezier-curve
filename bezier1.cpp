#include <iostream>
#include <vector>
#include <glm\glm.hpp>

using namespace std;

glm::vec3 lerp(float t, glm::vec3 a, glm::vec3 b)
{
	return (1 - t) * a + t * b;
}

vector<glm::vec3> iterate(float t, vector<glm::vec3> points)
{
	vector<glm::vec3> new_points;

	for (size_t i = 0; i < points.size() - 1; i++)
	{
		new_points.push_back(lerp(t, points[i], points[i + 1]));
	}

	return new_points;

}

vector<glm::vec3> bezier(vector<glm::vec3> points)
{
	const float delta_t = 0.05f;
	float t = 0;

	vector<glm::vec3> curve;

	while (t <= 1)
	{
		vector<glm::vec3> new_points = points;
		for (int i = 0; i < points.size() - 1; i++)
		{
			new_points = iterate(t, new_points);
		}

		curve.push_back(new_points[0]);
		t += delta_t;
	}

	return curve;
}

int main() 
{
	vector<glm::vec3> ps;
	vector<glm::vec3> c;
	int n;

	cin >> n;

	for (int i = 0; i < n; i++)
	{
		glm::vec3 p;
		cin >> p.x >> p.y >> p.z;
		ps.push_back(p);
	}

	cout << endl;

	c = bezier(ps);

	for (int i = 0; i < c.size(); i++)
	{
		cout << c[i].x << " " << c[i].y << " " << c[i].z << endl;
	}

	return 0;
}