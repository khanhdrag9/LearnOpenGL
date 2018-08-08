#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Draw polygon", NULL, NULL);
	if (window == NULL)
	{
		printf("Fail to create window!");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Fail to init Glad!");
		return -1;
	}
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//create shader
	const char* vs = "in vec3 aPos; void main(){gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}";
	const char* fsRed = "void main(){gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);}";
	const char* fsWhite = "void main(){gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);}";
	int succees;
	GLuint vShader, fShaderRed, fShaderWhite, programRed, programWhite;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vs, NULL);
	glCompileShader(vShader);

	fShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
	fShaderWhite = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShaderRed, 1, &fsRed, NULL);
	glShaderSource(fShaderWhite, 1, &fsWhite, NULL);
	glCompileShader(fShaderRed);
	glCompileShader(fShaderWhite);

	programRed = glCreateProgram();
	programWhite = glCreateProgram();
	glAttachShader(programRed, vShader);
	glAttachShader(programRed, fShaderRed);
	glAttachShader(programWhite, vShader);
	glAttachShader(programWhite, fShaderWhite);

	glLinkProgram(programRed);
	glGetProgramiv(programRed, GL_COMPILE_STATUS, &succees);
	if (!succees)
	{
		char info[255];
		glGetProgramInfoLog(programRed, 255, NULL, info);
		printf("ERROR::PROGRAM-RED:LINK\n%s\n", info);
	}
	glLinkProgram(programWhite);
	glGetShaderiv(programWhite, GL_COMPILE_STATUS, &succees);
	if (!succees)
	{
		char info[255];
		glGetProgramInfoLog(programWhite, 255, NULL, info);
		printf("ERROR::PROGRAM-WHITE:LINK\n%s\n", info);
	}

	glDeleteShader(vShader);
	glDeleteShader(fShaderRed);
	glDeleteShader(fShaderWhite);

	//vertex to GPU
	float s1[] = {
		-0.5, 1.0, 0.0,
		-1.0, 0.0, 0.0,
		-0.5, -1.0, 0.0,
		0.0, 0.0, 0.0
	};
	float s2[] = {
		0.5, 1.0, 0.0,
		0.0, 0.0, 0.0,
		0.5, -1.0, 0.0,
		1.0, 0.0, 0.0
	};
	unsigned int indices[] = { 0, 1,2 ,3 };

	GLuint VBO[2], VAO[2], EBO[2];
	
	glGenBuffers(2, VBO);
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, EBO);

	//SHAPE 1
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBindVertexArray(VAO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s1), s1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//SHAPE 2
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBindVertexArray(VAO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s2), s2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//loop
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//S1
		glUseProgram(programRed);
		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//S2
		glUseProgram(programWhite);
		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glfwSwapBuffers(window);
	}

	glDeleteBuffers(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(2, EBO);

	glfwTerminate();
	return 0;
}