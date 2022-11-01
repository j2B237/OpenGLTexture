#include "Shader.h"

/****************************************** PUBLIC *************************************/
Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	int success;
	char infoLog[512];

	GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);

	// catch error
	glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Program linking Error : " << infoLog << std::endl;
	}

	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


}

/**********************************************************************/
void Shader::activate()
{
	glUseProgram(ID);

}
/********************************************************************/

void Shader::setMat4(const std::string& name, glm::mat4 val)
{
	GLint location = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val));

}
/*****************************************************************/
void Shader::setInt(const std::string& name, int value) const
{
	GLint location = glGetUniformLocation(ID, name.c_str());
	glUniform1i(location, value );
}

/***************************************************************/

void Shader::setFloat(const std::string& name, float mixValue)
{
	GLint location = glGetUniformLocation(ID, name.c_str());
	glUniform1f(location, mixValue);
}

/*********************************** PROTECTED ******************************/
GLuint Shader::compileShader(const char* filePath, GLenum type)
{
	int success;
	char infoLog[512];

	GLuint shaderId = glCreateShader(type);
	std::string shaderSrc = loadShaderSrc(filePath);
	const GLchar* shader = shaderSrc.c_str();

	glShaderSource(shaderId, 1, &shader, NULL);
	glCompileShader(shaderId);

	//Catch error
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "Compile Shader Error: " << infoLog << std::endl;
	}

	return shaderId;

}

/****************************************************************/
std::string Shader::loadShaderSrc(const char* filename)
{
	std::ifstream file;
	std::stringstream buffer;
	std::string response = "";

	file.open(filename);

	if (file.is_open())
	{
		buffer << file.rdbuf();
		response = buffer.str();
	}
	else
	{
		std::cout << "Couldn't open " << filename << std::endl;
	}

	file.close();

	return response;
}

/***************************************************************************/