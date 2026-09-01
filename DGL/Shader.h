#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{

public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setColor(const char* name, float r, float g, float b, float a);
	void setMat4(const char* name, const glm::mat4& matrix);

	~Shader();

private:
	void checkCompileErrors(unsigned int shader, const char* type);
	std::string readFile(const char* filePath);
};

