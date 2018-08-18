#include "Shader.h"
#include <stdio.h>

Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::Init(const char* vs, const char* fs)
{
	vertexShader = loadShader(GL_VERTEX_SHADER, vs);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fs);
	program = loadProgram();
}
void Shader::Release()
{
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(program);
}
void Shader::UseProgram()
{
	glUseProgram(program);
}

void Shader::SetBoolUniform(char* name, bool value)
{
	glUniform1i(glGetUniformLocation(program, name), value);
}
void Shader::SetIntUniform(char* name, int value)
{
	glUniform1i(glGetUniformLocation(program, name), value);
}
void Shader::SetFloatUniform(char* name, float value)
{
	glUniform1f(glGetUniformLocation(program, name), value);
}

int Shader::loadShader(GLenum type,const char* path)
{
	GLuint shader = glCreateShader(type);
	GLint compiled;
	//create
	if (shader == 0)
	{
		printf("Fail to create Shader!\n");
		return 0;
	}

	//load source
	FILE* file;
	if (fopen_s(&file, path, "rb") != 0)
	{
		printf("Can not open your file!\n");
		return 0;
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* source = new char[size + 1];
	fread(source, sizeof(char), size, file);
	source[size] = 0;
	fclose(file);

	glShaderSource(shader, 1, (const char**)&source, NULL);	//(const char**) is copy 1 new text because we will delete source
	delete[]source;

	//compile
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		char info[255];
		glGetShaderInfoLog(shader, 255, NULL, info);
		printf("Error compile shader\n%s\n", info);
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
		printf("Fail to create program!\n");
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
		printf("Fail linking program\n%s\n", info);
		glDeleteProgram(program);
		return 0;
	}

	return program;
}