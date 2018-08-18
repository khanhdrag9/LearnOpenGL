#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdio.h>
#include <math.h>
#include "Shader.h"

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Draw Upside down triangle", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	Shader shader("../shader/vShader.vs", "../shader/fShader.fs");

	float vertex[] = {
		-0.5, 0.5, 0.3, 0.2, 0.1,
		0.5, 0.5, 0.5, 0.7, 0.7,
		0.0, -0.5, 0.8, 0.1, 1.0
	};
	unsigned int indices[] = { 0, 1, 2 };

	GLuint vbo, vao, ebo;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);	// 0 = glGetAttribLocation(program, "..");
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);	// 1 is aColor in vShader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	while (!glfwWindowShouldClose(window))
	{
		float timeValue = glfwGetTime();
		float value = sin(timeValue) / 2.f + 0.5f;
		shader.SetFloatUniform((char*)"xp", value);
		shader.SetFloatUniform((char*)"radius", value);

		glClear(GL_COLOR_BUFFER_BIT);
		shader.UseProgram();

		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ebo);
	shader.Release();

	glfwTerminate();

	return 0;
}