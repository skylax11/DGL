#pragma once
class Shader
{

public:
	unsigned int ID;

	Shader(const char* vertexSource, const char* fragmentSource);

	void use();

	~Shader();

private:
	void checkCompileErrors(unsigned int shader, const char* type);

};

