#ifndef __SHADER__
#define __SHADER__

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

class Shader
{
public:
	//Shader ID generated and used by OpenGL
	unsigned int ID;

	//Constructor
	Shader(const char* vertexPath, const char* fragmentPath);

	//Activates the Shader
	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
};
#endif