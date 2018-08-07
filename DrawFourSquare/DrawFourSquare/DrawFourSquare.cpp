// DrawFourSquare.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include <vector>

#define s(i) s##i

int _tmain(int argc, _TCHAR* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Draw Square", NULL, NULL);
	if (window == NULL)
	{
		printf("Fail to create Window\n!");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{ 
		printf("Fail to init Glad!");
		return -1;
	}

	//init
	glClearColor(0.7, 0.7, 0.7, 0.7);

	//create shader
	GLuint vShader, fShader[5], program[5];
	char* vs = "#version 330\n in vec3 aPos;void main(){gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}";
	char* fsRed = "void main(){gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);}";
	char* fsBlue = "void main(){gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);}";
	char* fsGreen = "void main(){gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);}";
	char* fsWhite = "void main(){gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);}";
	char* fsBlack = "void main(){gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);}";

	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vs, NULL);
	glCompileShader(vShader);

	for (int i = 0; i < 5; i++)
	{
		fShader[i] = glCreateShader(GL_FRAGMENT_SHADER);
	}
	glShaderSource(fShader[0], 1, &fsRed, NULL);
	glShaderSource(fShader[1], 1, &fsBlue, NULL);
	glShaderSource(fShader[2], 1, &fsGreen, NULL);
	glShaderSource(fShader[3], 1, &fsBlack, NULL);
	glShaderSource(fShader[4], 1, &fsWhite, NULL);

	for (int i = 0; i < 5; i++)
	{
		int success;
		char info[255];

		glCompileShader(fShader[i]);
		program[i] = glCreateProgram();
		glAttachShader(program[i], vShader);
		glAttachShader(program[i], fShader[i]);
		glLinkProgram(program[i]);
		glGetProgramiv(program[i], GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program[i], 255, NULL, info);
			printf("ERROR::PROGRAM::%d\n%s\n", i, info);
		}else
			glDeleteShader(fShader[i]);
	}
	glDeleteShader(vShader);
	//shape
	unsigned int indices[] = { 0, 1, 2, 0, 3, 2 };

	float s1[] =
	{
		-1.0, 1.0, 0.0,
		-1.0, 0.5, 0.0,
		-0.5, 0.5, 0.0,
		-0.5, 1, 0.0
	};

	float s2[] =
	{
		-1.0, -0.5, 0.0,
		-1.0, -1.0, 0.0,
		-0.5, -1.0, 0.0,
		-0.5, -0.5, 0.0
	};

	float s3[] =
	{
		0.5, -0.5, 0.0,
		0.5, -1.0, 0.0,
		1.0, -1.0, 0.0,
		1.0, -0.5, 0.0
	};

	float s4[] =
	{
		0.5, 1.0, 0.0,
		0.5, 0.5, 0.0,
		1.0, 0.5, 0.0,
		1.0, 1.0, 0.0
	};

	float s5[] =
	{
		-0.25, 0.25, 0.0,
		-0.25, -0.25, 0.0,
		0.25, -0.25, 0.0,
		0.25, 0.25, 0.25
	};

	

	GLuint VBO[5], VAO[5], EBO[5];
	glGenBuffers(5, VBO);
	glGenVertexArrays(5, VAO);

	for (int i = 0; i < 5; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBindVertexArray(VAO[i]);
		if (i == 0)
			glBufferData(GL_ARRAY_BUFFER, sizeof(s1), s1, GL_STATIC_DRAW);
		else if (i == 1)
			glBufferData(GL_ARRAY_BUFFER, sizeof(s2), s2, GL_STATIC_DRAW);
		else if (i == 2)
			glBufferData(GL_ARRAY_BUFFER, sizeof(s3), s3, GL_STATIC_DRAW);
		else if (i == 3)
			glBufferData(GL_ARRAY_BUFFER, sizeof(s4), s4, GL_STATIC_DRAW);
		else if (i == 4)
			glBufferData(GL_ARRAY_BUFFER, sizeof(s5), s5, GL_STATIC_DRAW);

		if (i == 0)
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)* 3, (void*)0);
		else
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < 5; i++)
		{
			glUseProgram(program[i]);
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(5 ,VBO);
	glDeleteBuffers(5, VAO);

	glfwTerminate();
	return 0;
}

