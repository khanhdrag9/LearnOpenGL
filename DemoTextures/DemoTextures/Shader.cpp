#include "Shader.h"
#include <stdio.h>


Shader::Shader(const char* vs, const char* fs)
{
	vShader = loadShader(vs, GL_VERTEX_SHADER);
	fShader = loadShader(fs, GL_FRAGMENT_SHADER);
	program = loadProgram();
}
Shader::~Shader()
{
}
void Shader::UseProgram()
{
	glUseProgram(program);
}
int Shader::GetAttribLoc(char* name)
{
	return glGetAttribLocation(program, name);
}
void Shader::SetFloatUniform(char* name, float value)
{
	int u = glGetUniformLocation(program, name);
	glUniform1f(u, value);
}
void Shader::SetIntUniform(char* name, int value)
{
	int u = glGetUniformLocation(program, name);
	glUniform1i(u, value);
}
void Shader::Release()
{
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	glDeleteProgram(program);
}

int Shader::loadShader(const char* path, GLenum type)
{
	GLuint shader = glCreateShader(type);
	int compiled;

	if (shader == 0)
	{
		printf("Fail to create Shader!");
		return 0;
	}
	//load source
	FILE* file;
	if (fopen_s(&file, path, "rb") != 0)
	{
		printf("Can not open your file!");
		return 0;
	}

	//count size of text
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);	//set cursor file to head file

	char* source = new char[size + 1];
	fread(source, sizeof(char), size, file);
	source[size] = 0;
	fclose(file);

	//setup shader
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		char info[255];
		glGetShaderInfoLog(shader, 255, NULL, info);
		printf("Fail compiling shader:\n%s\n", info);
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}
int Shader::loadProgram()
{
	GLuint program = glCreateProgram();
	int linked;
	if (program == 0)
	{
		printf("Fail to create program!");
		return 0;
	}

	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		char info[255];
		glGetProgramInfoLog(program, 255, NULL, info);
		printf("Fail linking program:\n%s\n", info);
		glDeleteProgram(program);
		return 0;
	}
	return program;
}
