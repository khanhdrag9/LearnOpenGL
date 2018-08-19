#pragma once
#include "glad/glad.h"
class Shader
{
	int loadShader(const char* path, GLenum type);
	int loadProgram();

	GLuint vShader, fShader, program;
public:
	Shader(const char* vs, const char* fs);
	~Shader();

	int GetAttribLoc(char* name);
	void SetFloatUniform(char* name, float value);
	void SetIntUniform(char* name, int value);
	void UseProgram();
	void Release();
};

