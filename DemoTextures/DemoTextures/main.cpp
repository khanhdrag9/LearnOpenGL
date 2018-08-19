#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "SOIL2/SOIL2.h"
#include <stdio.h>
#include "Shader.h"
#include "SOIL2/stb_image.h"
#include <math.h>

void OnPress(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Demo texture", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	Shader shader("vs.vs", "fs.fs");

	//coord
	float vertices[] = {
		0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = { 0, 1 , 2, 3};

	GLuint vbo, vao, ebo;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

	int pos = shader.GetAttribLoc((char*)"aPos");
	int color = shader.GetAttribLoc((char*)"aColor");
	int uv = shader.GetAttribLoc((char*)"aUV");

	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(uv);
	glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//texture1
	stbi_set_flip_vertically_on_load(true);
	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//because use clamp to boreder so we must set color for border
	//float borderColor[] = { 0.7, 0.2, 0.3, 1.0 };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load image 
	
	int width, height, nrChannels;
	//unsigned char* data = SOIL_load_image("../Welcom.jpg", &width, &height, &nrChannels, SOIL_LOAD_AUTO);
	unsigned char* data = SOIL_load_image("../wall.jpg", &width, &height, &nrChannels, SOIL_LOAD_AUTO);

	if (data)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else
		printf("Fail to load texture");

	SOIL_free_image_data(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//texture2
	GLuint texture2;
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = SOIL_load_image("../awesomeface.png", &width, &height, &nrChannels, SOIL_LOAD_AUTO);
	if (data)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		printf("Fail to load texture!");
	stbi_set_flip_vertically_on_load(true);

	SOIL_free_image_data(data);


	glClearColor(0.9, 0.8, 0.7, 1.0);
	while (!glfwWindowShouldClose(window))
	{
		OnPress(window);	//press to out

		//move
		float timeValue = glfwGetTime();
		float value = sin(timeValue) / 2.f + 0.5f;
		float value2 = cos(timeValue) / 2.f + 0.5f;
		shader.SetFloatUniform((char*)"x", value);
		shader.SetFloatUniform((char*)"y", value2);
		shader.UseProgram();
		shader.SetIntUniform((char*)"ourTexture", 0);
		shader.SetIntUniform((char*)"ourTexture2", 1);

		//bind and draw
		
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	shader.Release();
	glfwTerminate();
	return 0;
}