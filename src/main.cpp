// Main Source File for drawing a playground in OpenGL
// Authors -  Asutosh Sistla, Kushal Agrawal, Suchit Kar
// Date of Completion - 09/11/2017

#include <iostream>
#include <fstream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "scene.h"
using namespace std;


// Constants
const unsigned int SCR_W = 700;
const unsigned int SCR_H = 700;
const unsigned int SCR_MIN_W = 300;
const unsigned int SCR_MIN_H = 300;
const unsigned int SCR_MAX_W = 750;
const unsigned int SCR_MAX_H = 750;
const char* WINDOW_TITLE = "Interactive Bezier Curve";

// Pointer to a GLFWwindow object
GLFWwindow* window;

// Pointer to GLFWcursor objects
GLFWcursor* squareCursor;
GLFWcursor* arrowCursor;

// Pointer to a Scene object
Scene* myScene;

// Wireframe/fill mode specifier
bool wireframeMode = true;

// Curve/surface mode specifier
bool curveMode = true;

// Toggle wireframe/fill display mode
void toggleDisplayMode()
{
	if (wireframeMode)
	{
		// Enable polygon fill mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		// Enable polygon wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	wireframeMode = !wireframeMode;
}

// Toggle curve/surface mode
void toggleMode()
{
	if (curveMode)
	{
		// Disable cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		// Enable cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (wireframeMode)
		{
			toggleDisplayMode();
		}
	}
	curveMode = !curveMode;
}

// Write mesh to .off file
bool writeMesh(Mesh mesh)
{
	ofstream file;
	file.open("../mesh.off");

	if (file.is_open())
	{
		file << "OFF" << endl << endl;
		file << mesh.vertices.size() << " " << mesh.indices.size() / 3 << endl;

		for (size_t i = 0; i < mesh.vertices.size(); i++)
		{
			file << mesh.vertices[i].Position.x << " " << mesh.vertices[i].Position.y << " "
				<< mesh.vertices[i].Position.z << endl;
		}

		for (size_t i = 0; i < mesh.indices.size(); i += 3)
		{
			file << 3 << " " << mesh.indices[i] << " " << mesh.indices[i + 1] << " "
				<< mesh.indices[i + 2] << endl;
		}

		file.close();

		return true;
	}

	return false;
}

// Callback Function for Framebuffer Resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// ****************************************
// ********* Keypress Processing **********
// - Esc key closes the window
// - Space key toggles curve/surface mode
// - Return key writes current mesh to .off
// ****************************************
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		toggleMode();
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		if (writeMesh(myScene->bezierSurface))
		{
			cout << "Surface of revolution about Y axis written to mesh.off." << endl;
		}
		else
		{
			cout << "Write failed." << endl;
		}
	}
}

// *******************************************
// ********** Mouse Button Processing ********
// - Left Mouse Button adds control points
// - Drag and drop control points by
//   holding down the Left Mouse Button
// - Right Mouse Button deletes control points
// *******************************************
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (curveMode)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		xpos -= (width / 2);
		ypos = (height / 2 - ypos);
		xpos /= (width / 2);
		ypos /= (height / 2);
		glm::vec3 pos((float)xpos, (float)ypos, 0.0f);
		vector<glm::vec3>::iterator controlPoint = myScene->findControlPoint(pos);
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			if (controlPoint == myScene->controlPoints.end())
			{
				myScene->addControlPoint(pos);
			}
			else
			{
				myScene->controlPoints.erase(controlPoint);
				glfwSetCursor(window, squareCursor);
				myScene->picked = true;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			if (myScene->picked)
			{
				myScene->addControlPoint(pos);
				glfwSetCursor(window, arrowCursor);
				myScene->picked = false;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			if (controlPoint != myScene->controlPoints.end())
			{
				myScene->controlPoints.erase(controlPoint);
			}
		}
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{
		toggleDisplayMode();
	}
}

// OpenGL Initialization
bool initializeOpenGL()
{
	// GLFW Initialization
	if (!glfwInit())
	{
		cout << "Initialization failed" << endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window (default: 600p) and context
	window = glfwCreateWindow(SCR_W, SCR_H, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		cout << "Window or context creation failed" << endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// Register callback function for Window Resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Register callback function for keypresses
	glfwSetKeyCallback(window, key_callback);

	// Register callback function for mouse button press
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Fix aspect ratio to 16:9
	glfwSetWindowAspectRatio(window, 1, 1);

	// Set minimum limit for Window Size to 300p and maximum to 700p
	glfwSetWindowSizeLimits(window, SCR_MIN_W, SCR_MIN_H, SCR_MAX_W, SCR_MAX_H);

	//  Load OpenGL function pointers using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	//// Enable backface culling
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	// Viewport Settings
	glViewport(0, 0, SCR_W, SCR_H);

	// Create square cursor
	unsigned char pixels[8 * 8 * 4];
	memset(pixels, 0xff, sizeof(pixels));
	GLFWimage image;
	image.width = 8;
	image.height = 8;
	image.pixels = pixels;
	squareCursor = glfwCreateCursor(&image, 0, 0);

	arrowCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

	return true;
}

int main()
{
	// OpenGL Initialization
	if (!initializeOpenGL())
	{
		return -1;
	}

	// Create Scene object myScene
	myScene = new Scene();

	// **************** Render Loop ****************
	while (!glfwWindowShouldClose(window))
	{
		// ******** Rendering Commands ********
		// Clear buffers
		glm::vec3 backgroundColor = glm::vec3(0.404f, 0.684f, 0.900f);
		glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw objects
		myScene->drawYAxis();
		
		if (curveMode)
		{
			myScene->drawControlPoints();
			if (myScene->controlPoints.size() > 1)
			{
				myScene->drawBezierCurve();
			}
		}
		else
		{
			if (myScene->controlPoints.size() > 1)
			{
				myScene->drawBezierSurface();
			}
		}
		// ****** End Rendering Commands ******

		// Check for events, then swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// Deallocation of Resources
	delete myScene;
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}