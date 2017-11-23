#include <math.h>
#include "mesh.h"
#include "bezier1.h"
#include "surface.h"

using namespace std;

// Return a mesh that defines a surface of revolution about the Y-axis
Mesh Surface::getSurface(vector<glm::vec3> controlPoints, float delta_a)
{
	vector<glm::vec3> curve = bezier::bezier(controlPoints);

	vector<Vertex> vertices;
	vector<unsigned int> indices;

	Vertex p;
	float angle = 0;
	int times = (int)round(360.0f / delta_a);
	while (times--)
	{
		for (size_t i = 0; i < curve.size(); i++)
		{
			p.Position = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(curve[i], 1.0f);

			vertices.push_back(p);
		}
		angle += delta_a;
	}

	for (size_t i = 0; i < vertices.size(); i += curve.size())
	{
		for (size_t j = i; j - i + 1 < curve.size(); j++)
		{
			indices.push_back(j);
			indices.push_back((j + curve.size() + 1) % vertices.size());
			indices.push_back((j + curve.size()) % vertices.size());
			indices.push_back(j);
			indices.push_back(j + 1);
			indices.push_back((j + curve.size() + 1) % vertices.size());
		}
	}
	
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		glm::vec3 vec1 = vertices[indices[i + 1]].Position - vertices[indices[i]].Position;
		glm::vec3 vec2 = vertices[indices[i + 2]].Position - vertices[indices[i]].Position;
		glm::vec3 faceNormal = glm::normalize(glm::cross(vec2, vec1));
		vertices[indices[i]].Normal += faceNormal;
		vertices[indices[i + 1]].Normal += faceNormal;
		vertices[indices[i + 2]].Normal += faceNormal;
	}
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].Normal = glm::normalize(vertices[i].Normal);
	}

	Mesh mesh(vertices, indices);
	return mesh;
}