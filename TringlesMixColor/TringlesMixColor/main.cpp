#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include <stdio.h>
#include <math.h>


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Tringle mix Color", NULL, NULL);
	if (window == NULL)
	{
		printf("Fail to create Window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Fail to init glad!");
		return -1;
	}

	//shader
	const char* vs = "#version 330 core\nlayout(location = 0) in vec3 aPos;layout(location = 1)in vec3 aColor;out vec4 vColor;void main(){gl_Position = vec4(aPos, 1.0); vColor = vec4(aColor, 1.0);}";
	const char* fs = "#version 330 core\nout vec4 Color;in vec4 vColor;void main(){Color = vColor;}";

	GLuint vShader, fShader, program;
	int success;

	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vShader, 1, &vs, NULL);
	glShaderSource(fShader, 1, &fs, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info[255];
		glGetShaderInfoLog(vShader, 255, NULL, info);
		printf("ERROR::VERTEX:SHADER\n%s\n", info);
	}
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info[255];
		glGetShaderInfoLog(fShader, 255, NULL, info);
		printf("ERROR::FRAGMENT:SHADER\n%s\n", info);
	}
	
	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info[255];
		glGetShaderInfoLog(program, 255, NULL, info);
		printf("ERROR::PROGRAM\n%s\n", info);
	}
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	//vertex
	float vertices[] = 
	{
		0.0, 0.5, 0.0, 1.0, 0.0, 0.0,
		-0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
		0.5, -0.5, 0.0, 0.0, 0.0, 1.0
	};
	unsigned int indices[] = { 0, 1, 2 };

	GLuint VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	int pos = glGetAttribLocation(program, "aPos");
	int color = glGetAttribLocation(program, "aColor");

	if (pos != -1)
	{
		glEnableVertexAttribArray(pos);
		glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, sizeof(float)* 6, (void*)0);
	}
	if (color != -1)
	{
		glEnableVertexAttribArray(color);
		glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, sizeof(float)* 6, (void*)(sizeof(float)* 3));
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		float time = glfwGetTime();
		float colorValue = sin(time) / 2.f + 0.5f;


		glUseProgram(program);
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT,0);

		glBindVertexArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(program);
	glfwTerminate();
	return 0;
}