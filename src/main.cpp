#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include "Shader.h"

float const SCREEN_WIDTH = 800.0;
float const SCREEN_HEIGHT = 600.0;
float const CAMERA_DISTANCE = -15.0f;
int const NUM_OF_CUBES = 6;
float const ANGLE = 18;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int* loadTextures();
std::string getTextureString(int index);

glm::vec3 cubePositions[] =
{
	glm::vec3(0, 0, 0),
	glm::vec3(-5, -3, -3),
	glm::vec3(2, 4, 5),
	glm::vec3(0.5f, -3, -2.5f),
	glm::vec3(3, 3, -2),
	glm::vec3(-3, -2.5f, 6)
};

float vertices[] = {
		  	//Front Face					//Tex Coords
		  //Top Left - 0							
		   -1.0f, 1.0f, 1.0f,			0.0f, 0.333f,
		  //Bottom Left - 1
		    -1.0f, -1.0f, 1.0f,			0.0f, 0.0f,
		  //Top Right - 2
		    1.0f,  1.0f, 1.0f,			0.5f, 0.333f,
		  //Bottom Right - 3
		    1.0f,  -1.0f, 1.0f,			0.5f, 0.0f,

		  	//Back Face
		  //Top Left - 4							
		   -1.0f, 1.0f, -1.0f,			1.0f, 0.333f,
		  //Bottom Left - 5
		   -1.0f, -1.0f, -1.0f,			1.0f, 0.0f,
		  //Top Right - 6
		   1.0f,  1.0f, -1.0f,			0.5f, 0.333f,
		  //Bottom Right - 7
		   1.0f,  -1.0f, -1.0f,			0.5f, 0.0f,

		  	//Left Face
		  //Front Top - 8							
		   -1.0f, 1.0f, 1.0f,			0.0f, 0.666f,
		  //Front Bottom - 9
		   -1.0f, -1.0f, 1.0f,			0.0f, 0.333f,
		  //Back Top - 10
		   -1.0f,  1.0f, -1.0f,			0.5f, 0.666f,
		  //Back Bottom - 11
		   -1.0f,  -1.0f, -1.0f,		0.5f, 0.333f,

		  	//Right Face
		  //Front Top - 12							
		   1.0f, 1.0f, 1.0f,				0.5f, 0.666f,
		  //Front Bottom - 13
		   1.0f, -1.0f, 1.0f,			0.5f, 0.333f,
		  //Back Top - 14
		   1.0f,  1.0f, -1.0f,			1.0f, 0.666f,
		  //Back Bottom - 15
		   1.0f,  -1.0f, -1.0f,			1.0f, 0.333f,

		  	 //Top Face
		  //Front Left - 16							
		   -1.0f, 1.0f, 1.0f,			0.0f, 0.666f,
		  //Front Right - 17
		   1.0f, 1.0f, 1.0f,				0.5f, 0.666f,
		  //Back Left - 18
		   -1.0f,  1.0f, -1.0f,			0.0f, 1.0f,
		  //Back Right - 19
		   1.0f,  1.0f, -1.0f,			0.5f, 1.0f,

		  	//Bottom Face
		  //Front Left - 20							
		   -1.0f, -1.0f, 1.0f,			0.5f, 1.0f,
		  //Front Right  - 21
		   1.0f, -1.0f, 1.0f,			1.0f, 1.0f,
		  //Back Left - 22
		   -1.0f,  -1.0f, -1.0f,		0.5f, 0.666f,
		  //Back Right - 23
		   1.0f,  -1.0f, -1.0f,			1.0f, 0.666f,
};

unsigned int indices[] = {
	//Front Face
	0, 1, 2,			2, 3, 1,
	//Back Face
	4, 5, 6,			6, 7, 5,
	//Left Face
	10, 11, 8,		8, 9, 11,
	//Right Face
	14, 15, 12,		12, 13, 15,
	//Top Face
	19, 18, 17,		17, 16, 18,
	//Bottom Face
	23, 22, 21,		20, 21, 22,
};

int main()
{
	glfwInit();

	//First argument of glfwWindowHint says which option we want to congiure,
	//and the second argument sets the value of option.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creates a window and a corresponding OpenGL context, however the context is
	//NOT the current context until we set it as the current context.
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Sets the context of our window as the main context on the current thread
	glfwMakeContextCurrent(window);

	//Initialization of GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Tell OpenGL the size of the rendering window so OpenGL
	//knows how we want to display the data and coordinates with respect to the window.
	//y = 0 is bottom of viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex Buffer Object: stores a large number of vertices on the GPUs memory for later use
	//Memory of the VBO is managed by OpenGL, so we do not get a pointer. Instead, we get a positive int
	//to reference our VBO with. We use this ID to make our VBO active and destroy it.
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//Binds the newly created buffer to the GL_ARRAY_BUFFER target of OpenGL. OpenGL allows us to
	//bind to several buffers at once as long as they have different buffer types
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Any buffer calls we make to the GL_ARRAY_BUFFER target will be used to configure the currently bound buffer of VBO
	//Copies the vertex data into the buffers memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Creation of Element Buffer Object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Setting up Vertex Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	Shader basicShader("shaders/basic.vs", "shaders/basic.fs");
	basicShader.use();

	//The projection matrix
	glm::mat4 projMatrix = glm::mat4(1.0f);
	projMatrix = glm::perspective(glm::radians(45.0f), (SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 100.0f);
	basicShader.setMat4("projection", projMatrix);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	unsigned int* textureData = loadTextures();

	//The Render Loop
	//glfwWindowShouldClose checks if our window has been instructed to close,
	//and if it has, we exit the render loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//The view matrix, from world coordinates to where our 'camera' is
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		float camX = (sin(glfwGetTime()) * CAMERA_DISTANCE);
		float camZ = (cos(glfwGetTime()) * CAMERA_DISTANCE);
		viewMatrix = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		basicShader.setMat4("view", viewMatrix);

		glBindVertexArray(VAO);
		for (int i = 0; i < NUM_OF_CUBES; i++)
		{
			glBindTexture(GL_TEXTURE_2D, textureData[i]);

			//The model matrix, from local to world coordinates
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
			float angle = (float)glfwGetTime() * (ANGLE * (i + 1));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			basicShader.setMat4("model", modelMatrix);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
		
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//clean up any allocated resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(basicShader.ID);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

unsigned int* loadTextures()
{
	unsigned int* textureData = new unsigned int[NUM_OF_CUBES];
	glGenTextures(NUM_OF_CUBES, textureData);

	for (int i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textureData[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(getTextureString(i).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	return textureData;
}

std::string getTextureString(int index)
{
	std::string texture = "textures/";

	switch (index)
	{
	case 0:
		return texture + "Burlington.jpg";

	case 1:
		return texture + "Adventures.jpg";

	case 2:
		return texture + "Memories.jpg";

	case 3:
		return texture + "Life.jpg";

	case 4:
		return texture + "Mayhem.jpg";

	case 5:
		return texture + "Ego.jpg";
	}
}