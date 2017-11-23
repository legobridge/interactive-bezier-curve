#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    FragPos = aPos;
    Normal = aNormal;
    gl_Position = vec4(FragPos, 1.0);
}