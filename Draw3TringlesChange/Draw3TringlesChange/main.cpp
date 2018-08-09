#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

void BindVertex(float *vertex, unsigned int *indices, GLuint& vbo, GLuint& vao, GLuint& ebo, GLuint program);
void Draw(GLuint VAO)
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 2);
	glBindVertexArray(0);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Draw 3 tringles change colors", NULL, NULL);
	if (window == NULL)
	{
		printf("Fail to create Window!");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Fail to load Glad");
		return -1;
	}

	//shader
	const char* vs = "#version 330 core\nlayout(location = 0) in vec2 aPos;void main(){gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);}";
	const char* fs = "#version 330 core\nout vec4 Color;uniform vec4 uColor;void main(){Color = vec4(1.0, 0.0, 1.0, 1.0);}";

	int success;

	GLuint vShader, fShader, program[3];
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vs, NULL);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info[255];
		glGetShaderInfoLog(vShader, 255, NULL, info);
		printf("ERROR::VERTEX::SHADER\n%s\n", info);
	}
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fs, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info[255];
		glGetShaderInfoLog(fShader, 255, NULL, info);
		printf("ERROR::FRAGMENT::SHADER\n%s\n", info);
	}

	for (int i = 0; i < 3; i++)
	{
		program[i] = glCreateProgram();
		glAttachShader(program[i], vShader);
		glAttachShader(program[i], fShader);
		glLinkProgram(program[i]);
		glGetProgramiv(program[i], GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char info[255];
			glGetProgramInfoLog(program[i], 255, NULL, info);
			printf("ERROR::PROGRAM\n%s\n", info);
		}
	}
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	
	//vertex
	float s1[] = {
		-1.0, 0.0,
		-0.75, -0.5,
		-0.5, 0.0,
		-0.75, 0.25
	};
	float s2[] = {
		-0.25, 0.0,
		0, -0.25, 
		0.25, 0.0,
		0.0, 0.25
	};
	float s3[] = {
		0.75, 0.0, 
		0., -0.25, 
		-0.5, 0.0,
		-0.75, 0.25
	};
	unsigned int indices[] = { 0, 1,2,3 };

	GLuint VBOs[3], VAOs[3], EBOs[3];
	BindVertex(s1, indices, VBOs[0], VAOs[0], EBOs[0], program[0]);
	BindVertex(s2, indices, VBOs[1], VAOs[1], EBOs[1], program[1]);
	BindVertex(s3, indices, VBOs[2], VAOs[2], EBOs[2], program[2]);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		
		for (int i = 0; i < 3; i++)
		{
			Draw(VAOs[0]);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void BindVertex(float *vertex, unsigned int *indices, GLuint& vbo, GLuint& vao, GLuint& ebo, GLuint program)
{
	glUseProgram(program);

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, vertex, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 4, indices, GL_STATIC_DRAW);

	int pos = glGetAttribLocation(program, "aPos");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}