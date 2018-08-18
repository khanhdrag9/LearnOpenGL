#pragma once
#include "glad/glad.h"
class Shader
{
	GLuint vertexShader, fragmentShader, program;

	int loadShader(const char* path, GLenum type);
	int loadProgram();
public:
	Shader(const char* vs, const char* fs);
	~Shader();

	void UseProgram();
	void Release();
	void SetFloatUniform(char* name, float value);
};

