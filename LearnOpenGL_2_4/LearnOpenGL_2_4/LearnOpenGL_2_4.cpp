// LearnOpenGL_2_4.cpp : Defines the entry point for the console application.
//

//DRAW 2 TRIANGLES NEAR OTHERS

#include "stdafx.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void KeyPress(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int _tmain(int argc, _TCHAR* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Exercies", NULL, NULL);
	if (window == NULL)
	{
		printf("FAIL to create window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("FAIL to init GLAD!\n");
		return -1;
	}

	//register callback
	glfwSetFramebufferSizeCallback(window ,framebuffer_size_callback);

	glClearColor(1.f, 1.f, 1.f, 1.f);

	//create shader
	char* vs = "in vec3 aPos;\nvoid main(){gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}";
	char* fs = "void main(){gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);}";
	int cSuccessShader;
	char info[512];

	GLuint vShader, fShader, program;

	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vs, NULL);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &cSuccessShader);
	if (!cSuccessShader)
	{
		glGetShaderInfoLog(vShader, 512, NULL, info);
		printf("ERROR::SHADER::VERTEX\n%s\n", info);
	}

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fs, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &cSuccessShader);
	if (!cSuccessShader)
	{
		glGetShaderInfoLog(fShader, 512, NULL, info);
		printf("ERROR::SHADER::FRAGMENT\n%s\n", info);
	}

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_COMPILE_STATUS, &cSuccessShader);
	if (!cSuccessShader)
	{
		glGetProgramInfoLog(program, 512, NULL, info);
		printf("ERROR::SHADER::PROGRAM\n%s\n", info);
	}
	//afer link, delete shader
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	//Bind to GPU
	float vertices[] = {
		-1.f, -0.5f, 0.f,
		0.f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0,
		1.f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0
	};

	GLuint VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	GLuint indices[] = { 1, 2, 0, 1, 3, 4 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		//Wireframe mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glUseProgram(program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		KeyPress(window);	//function to listen key from keyboard( ESC to exit)
		glfwSwapBuffers(window);
		glfwPollEvents();	//listen event as input keyboard,...
	}

	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	
	glfwTerminate();
	return 0;
}

