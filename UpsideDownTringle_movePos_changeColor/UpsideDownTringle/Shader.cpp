#include "Shader.h"
#include <stdio.h>

Shader::Shader(const char* vs, const char* fs)
{
	vertexShader = loadShader(vs, GL_VERTEX_SHADER);
	fragmentShader = loadShader(fs, GL_FRAGMENT_SHADER);
	program = loadProgram();
}
Shader::~Shader()
{
}
void Shader::UseProgram()
{
	glUseProgram(program);
}
void Shader::SetFloatUniform(char* name, float value)
{
	glUniform1f(glGetUniformLocation(program, name), value);
}
void Shader::Release()
{
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(program);
}

int Shader::loadShader(const char* path, GLenum type)
{
	GLuint shader;
	int compiled;

	shader = glCreateShader(type);
	if (shader == 0)
	{
		printf("Fail to create Shader!\n");
		return 0;
	}

	//load shader
	FILE* file;
	if (fopen_s(&file, path, "rb") != 0)
	{
		printf("Can not open your file : %s\n", path);
		return 0;
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* source = new char[size + 1];
	fread(source, sizeof(char), size, file);
	source[size] = 0;
	fclose(file);

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
	GLuint program;
	int linked;

	program = glCreateProgram();
	if (program == 0)
	{
		printf("Fail to create Program\n");
		return 0;
	}
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
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

