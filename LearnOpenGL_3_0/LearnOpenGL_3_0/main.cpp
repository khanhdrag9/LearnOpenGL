
#include "glad\glad.h"
#include "GLFW\glfw3.h"

#include <stdio.h>

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn Open GL part3-lession1", NULL, NULL);
	if (window == NULL)
	{
		printf("Fail to create Window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Fail to init Glad\n");
		return -1;
	}
	glClearColor(0.8, 0.5, 0.8, 1.0);

	//init shader

	//loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);


		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}