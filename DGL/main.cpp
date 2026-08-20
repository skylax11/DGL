#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Mesh.h"

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
"}\0";

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "DGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Pencere olusturulamadi\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD ile OpenGL fonksiyonlarini yukle
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD baslatilamadi\n";
        return -1;
    }

    glViewport(0, 0, 800, 600);

    float r{ 0.1f };
    float g{ 0.6f };
    float b{ 0.3f };

    std::vector<float> vertices =
    {
        -0.5f,  -0.5f,   0,
        0.5f,   -0.5f,   0,
        0,      0.5f,    0
    };

    Mesh triangle(vertices);

    std::unique_ptr<Shader> shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);


    while (!glfwWindowShouldClose(window)) {
        glClearColor(r, g, b, 1.0f);  
        glClear(GL_COLOR_BUFFER_BIT);

        //(*shader).use();
        shader->use();
        triangle.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

