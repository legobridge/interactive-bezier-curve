#include <iostream>
#include <algorithm>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "scene.h"
#include "bezier1.h"

using namespace std;

// Constructor definition
Scene::Scene()
	: lineShader("../src/shader/line.vs", "../src/shader/line.fs")
	, surfaceShader("../src/shader/surface.vs", "../src/shader/surface.fs")
	, bezierSurface(surface.getSurface(controlPoints, 180.0f))
{
	picked = false;
	glGenVertexArrays(1, &lineVAO);
	glGenVertexArrays(1, &surfaceVAO);
}

// Find a control point on the given position
vector<glm::vec3>::iterator Scene::findControlPoint(glm::vec3 position)
{
	for (vector<glm::vec3>::iterator it = controlPoints.begin(); it != controlPoints.end(); it++)
	{
		if (glm::distance(position, *it) < 0.028f)
		{
			return it;
		}
	}
	return controlPoints.end();
}

// Add control point
void Scene::addControlPoint(glm::vec3 controlPoint)
{
	controlPoints.push_back(controlPoint);
	sort(controlPoints.begin(), controlPoints.end(),
		[](const glm::vec3 &lhs, const glm::vec3 &rhs)
	{
		return lhs.y < rhs.y;
	});
}

// Draw the Y-Axis
void Scene::drawYAxis()
{
	vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(0.0f, -0.9f, 0.0f));
	vertices.push_back(glm::vec3(0.0f, 0.9f, 0.0f));
	glm::vec3 colorVector(0.0f, 0.8f, 0.0f);

	lineShader.use();
	glBindVertexArray(lineVAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	unsigned int objectColorLoc = glGetUniformLocation(lineShader.ID, "objectColor");

	glUniform3f(objectColorLoc, colorVector.x, colorVector.y, colorVector.z);

	glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

	glDeleteBuffers(1, &VBO);

	glBindVertexArray(0);
}

// Draw the control points
void Scene::drawControlPoints()
{
	glm::vec3 colorVector(0.8f, 0.6f, 0.7f);

	lineShader.use();
	glBindVertexArray(lineVAO);

	vector<unsigned int> indices = {
		0, 1, 2,
		0, 2, 3
	};
	
	for (size_t i = 0; i < controlPoints.size(); i++)
	{
		float cpx = controlPoints[i].x;
		float cpy = controlPoints[i].y;
		vector<glm::vec3> vertices = {
			glm::vec3(cpx - 0.02f * cos(glm::radians(45.0f)), cpy - 0.02f * sin(glm::radians(45.0f)), 0.0f),
			glm::vec3(cpx + 0.02f * cos(glm::radians(45.0f)), cpy - 0.02f * sin(glm::radians(45.0f)), 0.0f),
			glm::vec3(cpx + 0.02f * cos(glm::radians(45.0f)), cpy + 0.02f * sin(glm::radians(45.0f)), 0.0f),
			glm::vec3(cpx - 0.02f * cos(glm::radians(45.0f)), cpy + 0.02f * sin(glm::radians(45.0f)), 0.0f)
		};

		unsigned int VBO, EBO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		unsigned int objectColorLoc = glGetUniformLocation(lineShader.ID, "objectColor");

		glUniform3f(objectColorLoc, colorVector.x, colorVector.y, colorVector.z);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	glBindVertexArray(0);
}

// Draw the Bezier curve
void Scene::drawBezierCurve()
{
	vector<glm::vec3> vertices = bezier::bezier(controlPoints);
	glm::vec3 colorVector(0.8f, 0.0f, 0.1f);

	lineShader.use();
	glBindVertexArray(lineVAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	unsigned int objectColorLoc = glGetUniformLocation(lineShader.ID, "objectColor");

	glUniform3f(objectColorLoc, colorVector.x, colorVector.y, colorVector.z);

	glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

	glDeleteBuffers(1, &VBO);

	glBindVertexArray(0);
}

// Draw the Bezier surface
void Scene::drawBezierSurface()
{
	bezierSurface = surface.getSurface(controlPoints);

	vector<Vertex> vertices = bezierSurface.vertices;
	vector<unsigned int> indices = bezierSurface.indices;
	glm::vec3 colorVector(0.8f, 0.0f, 0.1f);

	// Use the compiled shader program
	surfaceShader.use();

	glBindVertexArray(surfaceVAO);

	unsigned int VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glm::vec3 lightPos = glm::vec3(1.0f, 3.0f, 0.0f);
	float lightVal = 1.0f;
	glm::vec3 lightColor = glm::vec3(lightVal, lightVal, lightVal);

	unsigned int lightPosLoc = glGetUniformLocation(surfaceShader.ID, "lightPos");
	unsigned int lightColorLoc = glGetUniformLocation(surfaceShader.ID, "lightColor");
	unsigned int objectColorLoc = glGetUniformLocation(surfaceShader.ID, "objectColor");

	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(objectColorLoc, colorVector.x, colorVector.y, colorVector.z);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glBindVertexArray(0);
}