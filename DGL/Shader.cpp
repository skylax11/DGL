#include "Shader.h"
#include <glad/glad.h>
#include <iostream>

Shader::Shader(const char* vertexSource, const char* fragmentSource) {

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

void Shader :: use()
{
	glUseProgram(ID);
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