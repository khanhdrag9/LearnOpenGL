// LearnOpenGL_2_4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "glad\glad.h"
#include "GLFW\glfw3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);


		glfwSwapBuffers(window);
		glfwPollEvents();	//listen event as input keyboard,...
	}


	glfwTerminate();
	return 0;
}

