#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>


#include "Shader.h"

/***************************************************************************/
void glfw_InitWindow();
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader& sh, glm::mat4& val, float& mixValue);

/***************************************************************************/


int main(int argc, char* argv[])
{
	std::cout << "Hello openGL World !!!" << std::endl;

	glfw_InitWindow();

	// OpenGL
	GLFWwindow* window = glfwCreateWindow(800, 600, "Texture tutorial", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "GLFW Error: failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//End 

	glfwMakeContextCurrent(window);

	// Glad
	if (! gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress) )
	{
		std::cout << "Glad Error: Failed to initialize Glad" << std::endl;
	}

	// End Glad

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);

	// Vertex definitions
	
	float vertices[] = {

		// Position			  // Color		      // Texture
		0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {
		0, 1, 3,			// First triangle
		2, 1, 3			//Second triangle
	
	};

	unsigned int VAO, VBO, EBO;
	float mixValue = 0.2f;


	// Generate buffers
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	
	// Bind buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Copy data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Set attributes pointers

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)) );
	glEnableVertexAttribArray(1);

	// Texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)) );
	glEnableVertexAttribArray(2);

	// Create instace shader
	Shader shader("vertex_shader.glsl", "fragment_shader.glsl");

	// Create texture
	unsigned int texture1, texture2;

	// Texture 1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);


	// Set texture wrapping filtering options on the currently bound texture object
	// Texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	

	// Load image
	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char *data = stbi_load("container.jpg", &width, &height, &nChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture " << std::endl;
	}

	stbi_image_free(data);


	// Texture 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Set the txture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//stbi_set_flip_vertically_on_load(false);
	// load image, create texture and generate Mipmaps
	data = stbi_load("ssd.png", &width, &height, &nChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 2" << std::endl;
	}

	stbi_image_free(data);


	// Activate shader program and texture units

	shader.activate();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	glm::mat4 trans = glm::mat4(1.0f);
	
	trans = glm::rotate(trans, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.activate();
	shader.setMat4("transform", trans);

	// Game loop
	while (!glfwWindowShouldClose(window) )
	{
		// Process input
		processInput(window, shader, trans, mixValue);

		// Render stuff
		glClearColor(0.2f, 0.4f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Set the texture mix value in the shader
		shader.setFloat("mixValue", mixValue);

		// Draw vertex
		shader.activate();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Catch events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VBO);
	glDeleteVertexArrays(1, &EBO);

	glfwTerminate();
	return 0;
}
/*******************************************************************************/
void glfw_InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

}

/*****************************************************************************/
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/**************************************************************************/

void processInput(GLFWwindow* window, Shader& sh, glm::mat4& val, float& mixValue)
{
	// Exit window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	// Rotate left direction around Z axis
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		val = glm::rotate(val, glm::radians((float)(glfwGetTime() / 100.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
		sh.activate();
		sh.setMat4("transform", val);
	}
	// Rotate right direction around Z axis
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		val = glm::rotate(val, glm::radians(-(float)(glfwGetTime() / 100.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
		sh.activate();
		sh.setMat4("transform", val);
	}
	// Rotate Up around X axis
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		val = glm::rotate(val, glm::radians((float)(glfwGetTime() / 100.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
		sh.activate();
		sh.setMat4("transform", val);
	}
	// Rotate Down around X axis
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		val = glm::rotate(val, glm::radians(-(float)(glfwGetTime() / 100.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
		sh.activate();
		sh.setMat4("transform", val);
	}
	// Disable or enable shader
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		sh.activate();
		glDisableVertexAttribArray(0);
		
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		sh.activate();
		glEnableVertexAttribArray(0);
	}
	// linear interpolate texture mix value between [0, 1]
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mixValue -= 0.001f;
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mixValue += 0.001f;
		if (mixValue >= 0.99f)
			mixValue = 1.0f;
		
	}
}
/**************************************************************************/