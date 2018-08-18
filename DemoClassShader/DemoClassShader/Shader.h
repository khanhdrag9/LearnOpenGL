#pragma once
#include "glad/glad.h"

class Shader
{
	int loadShader(GLenum type, const char* path);
	int loadProgram();
	GLuint vertexShader, fragmentShader, program;
public:
	Shader();
	~Shader();

	void Init(const char* vs, const char* fs);
	void UseProgram();
	void Release();

	void SetBoolUniform(char* name, bool value);
	void SetIntUniform(char* name, int value);
	void SetFloatUniform(char* name, float value);
};

