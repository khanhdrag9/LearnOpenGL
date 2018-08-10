#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <math.h>

void BindVertex(float *vertex, unsigned int *indices, GLuint& vbo, GLuint& vao, GLuint& ebo, GLuint program);
void Draw(GLuint& VAO, GLuint program)
{
	glUseProgram(program);

	static int count;
	float time = glfwGetTime();
	float color = sin(time) / 2.f + 0.5f;

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
	
	
	if (count == 0)
	{
		int uniform = glGetUniformLocation(program, "uColor1");
		glUniform4f(uniform, color, 0.0, 0.0, 1.0);
		count++;
	}
	if (count == 1)
	{
		int uniform = glGetUniformLocation(program, "uColor2");
		glUniform4f(uniform, 0.0, color, 0.0, 1.0);
		count++;
	}
	if (count == 2)
	{
		int uniform = glGetUniformLocation(program, "uColor3");
		glUniform4f(uniform, 0.0, 0.0, color, 1.0);
		count = 0;
	}

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
	const char* fs[3] = { "#version 330 core\nout vec4 Color;uniform vec4 uColor1;void main(){Color = uColor1;}",
		"#version 330 core\nout vec4 Color;uniform vec4 uColor2;void main(){Color = uColor2;}",
		"#version 330 core\nout vec4 Color;uniform vec4 uColor3;void main(){Color = uColor3;}"
	};

	int success;

	GLuint vShader, fShader[3], program[3];
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
	

	for (int i = 0; i < 3; i++)
	{
		fShader[i] = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fShader[i], 1, &fs[i], NULL);
		glCompileShader(fShader[i]);
		glGetShaderiv(fShader[i], GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char info[255];
			glGetShaderInfoLog(fShader[i], 255, NULL, info);
			printf("ERROR::FRAGMENT::SHADER\n%s\n", info);
		}

		program[i] = glCreateProgram();
		glAttachShader(program[i], vShader);
		glAttachShader(program[i], fShader[i]);
		glLinkProgram(program[i]);
		glGetProgramiv(program[i], GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char info[255];
			glGetProgramInfoLog(program[i], 255, NULL, info);
			printf("ERROR::PROGRAM\n%s\n", info);
		}
		glDeleteShader(fShader[i]);
	}
	glDeleteShader(vShader);
	
	
	
	//vertex
	float s1[] = {
		-1.0, 0.0,
		-0.75, -0.5,
		-0.5, 0.0,
		-0.75, 0.5
	};
	float s2[] = {
		-0.25, 0.0,
		0, -0.5, 
		0.25, 0.0,
		0.0, 0.5
	};
	float s3[] = {
		0.5, 0.0, 
		0.75, -0.5, 
		1.0, 0.0,
		0.75, 0.5
	};
	unsigned int indices[] = { 0, 1,2,3 };

	GLuint VBOs[3], VAOs[3], EBOs[3];
	BindVertex(s1, indices, VBOs[0], VAOs[0], EBOs[0], program[0]);
	BindVertex(s2, indices, VBOs[1], VAOs[1], EBOs[1], program[1]);
	BindVertex(s3, indices, VBOs[2], VAOs[2], EBOs[2], program[2]);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		
		for (int i = 0; i < 3; i++)
		{
			Draw(VAOs[i], program[i]);
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