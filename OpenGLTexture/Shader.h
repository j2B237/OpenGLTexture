#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int ID;
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	void activate();
	void setMat4(const std::string& name, glm::mat4 val);
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float mixValue);

protected:
	GLuint compileShader(const char* filePath, GLenum type);
	std::string loadShaderSrc(const char* filename);
};



#endif

