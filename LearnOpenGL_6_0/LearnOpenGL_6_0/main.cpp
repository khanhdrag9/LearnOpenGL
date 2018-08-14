#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include "SOIL2\SOIL2.h"
#include <stdio.h>


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1024, 760, "Learn  Open GL 6-0", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Fail to init GLAD!\n");
		return -1;
	}

	//textCoords

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 0.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);	//spectify color of boreder
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load image
	GLuint texture;
	int width, height, nrChannels;
	unsigned char* data = SOIL_load_image("wall.jpg", &width, &height, &nrChannels, 0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (data)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else
		printf("Fail to load texture\n");

	SOIL_free_image_data(data);

	//shader
	const char* vs = "#version 330 core\nlayout(location = 0) vec3 aPos;layout(location = 1)vec2 aUV;out vec2 vUV;void main(){gl_Position = aPos;vUV = aUV;}";
	const char* fs = "#version 330 core\n;out vec4 FragColor;in vec2 vUV;uniform sampler2D texture;void main(){FragColor = texture(texture, vUV);}";

	GLuint vShader, fShader, program;
	int success;

	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vs, NULL);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info[255];
		glGetShaderInfoLog(vShader, 255, NULL, info);
		printf("ERROR::VERTEX:SHADER\n\s\n", info);
	}

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fs, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info[255];
		glGetShaderInfoLog(fShader, 255, NULL, info);
		printf("ERROR::FRAGMENT:SHADER\n\s\n", info);
	}

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	//vertex
	float vertices[] = {
		// positions   // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
		-0.5f, 0.5f, 0.0f,  0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = { 0, 1, 2, 3 };

	GLuint VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int pos, uv;
	pos = glGetAttribLocation(program, "aPos");
	if (pos != -1)
	{
		glEnableVertexAttribArray(pos);
		glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
	}
	uv = glGetAttribLocation(program, "aUV");
	if (uv != -1)
	{
		glEnableVertexAttribArray(uv);
		glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);




		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}