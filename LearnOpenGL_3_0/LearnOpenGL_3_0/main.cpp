#include <stdio.h>
#include "glad/glad.h"
#include "GLFW\glfw3.h"
#include <math.h>

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL L3-Shaders", NULL, NULL);
	if (window == NULL)
	{
		printf("Fail to create window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Fail to init glad");
		return -1;
	}
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//shader
	const char* vs = "#version 330 core\nlayout(location = 0) in vec3 aPos;\nvoid main(){gl_Position = vec4(aPos, 1.0);}";
	const char* fs = "#version 330 core\nout vec4 Color;\n uniform vec4 aColor;void main(){Color = aColor;}";

	GLuint vShader, fShader, program;
	int success;

	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vs, NULL);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if(!success) printf("ERROR:VERXTEX::SHADER\n");

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fs, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if(!success) printf("ERROR:FRAGMENT::SHADER\n");

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info[255];
		glGetProgramInfoLog(program, 255, NULL, info);
		printf("ERROR::PROGRAM\n%s\n", info);
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	//vertex
	float vertices[] = {
		-0.5, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.5, 0.0, 0.0
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

	int l = glGetAttribLocation(program, "aPos");
	glVertexAttribPointer(l, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(l);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	

	//loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	

		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.f + 0.5f;

		glUseProgram(program);
		int color = glGetUniformLocation(program, "aColor");
		glUniform4f(color, 1.0, greenValue, 0.0, 1.0);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}