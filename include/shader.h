#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader
{
public:
    unsigned int ID;

    // Constructor generates the shader on the fly
	Shader(const char* vertexPath, const char* fragmentPath);

    // Activate the shader
	void use();

private:
    // Utility function for checking shader compilation/linking errors.
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif