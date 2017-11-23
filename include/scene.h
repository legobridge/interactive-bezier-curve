#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"
#include "mesh.h"
#include "surface.h"

class Scene
{
public:

	// The line shader object
	Shader lineShader;

	// The surface shader object
	Shader surfaceShader;

	// Control points
	std::vector<glm::vec3> controlPoints;

	// Have we picked up a control point?
	bool picked;

	// Surface object
	Surface surface;

	// Mesh of the Bezier surface
	Mesh bezierSurface;

	// Vertex array objects to draw from
	unsigned int lineVAO;
	unsigned int surfaceVAO;

	// Constructor
	Scene();

	// Draw the control points
	void drawControlPoints();

	// Find a control point on the given position
	std::vector<glm::vec3>::iterator findControlPoint(glm::vec3 position);

	// Add control point
	void addControlPoint(glm::vec3 controlPoint);

	// Draw the Y-Axis
	void drawYAxis();

	// Draw the Bezier curve
	void drawBezierCurve();

	// Draw the Bezier surface
	void drawBezierSurface();
};

#endif