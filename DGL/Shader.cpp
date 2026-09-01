#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX");

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

std::string Shader:: readFile(const char* filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cout << "dosya acilamadi." << filePath << "\n";
        return "";
    }

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}

void Shader :: use()
{
	glUseProgram(ID);
}

void Shader::setColor(const char* name, float r, float g, float b, float a)
{
    int location = glGetUniformLocation(ID, name);
    glUniform4f(location, r, g, b,a);
}

void Shader::setMat4(const char* name, const glm::mat4& matrix) {
    int location = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}


Shader::~Shader() {
	glDeleteProgram(ID);
}

void Shader::checkCompileErrors(unsigned int shader, const char* type) {
    int success;
    char infoLog[512];

    if (type != std::string("PROGRAM")) {
        // Shader derleme hatasý kontrolü
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "SHADER DERLEME HATASI (" << type << "):\n" << infoLog << "\n";
        }
    }
    else {
        // Program link hatasý kontrolü
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << "PROGRAM LINK HATASI:\n" << infoLog << "\n";
        }
    }
}