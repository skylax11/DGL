#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Mesh.h"
#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>
#include <cstdlib>


const char* vertexPath = "shaders/basic.vert";
const char* fragmentPath = "shaders/basic.frag";

void printMatrisOnConsole(glm::mat4& model, int& second, int interval);

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

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);

    float r{ 0.1f };
    float g{ 0.6f };
    float b{ 0.3f };

    std::vector<float> triangleVertices =
    {
        -0.5f,  -0.5f,   0,
        0.5f,   -0.5f,   0,
        0,      0.5f,    0
    };

    std::vector<float> cubeVertices = {

        -0.5f,-0.5f,-0.5f, 1.0f,0.0f,0.0f,  0.5f,-0.5f,-0.5f, 1.0f,0.0f,0.0f,  0.5f, 0.5f,-0.5f, 1.0f,0.0f,0.0f,
         0.5f, 0.5f,-0.5f, 1.0f,0.0f,0.0f, -0.5f, 0.5f,-0.5f, 1.0f,0.0f,0.0f, -0.5f,-0.5f,-0.5f, 1.0f,0.0f,0.0f,

        -0.5f,-0.5f, 0.5f, 0.0f,1.0f,0.0f,  0.5f,-0.5f, 0.5f, 0.0f,1.0f,0.0f,  0.5f, 0.5f, 0.5f, 0.0f,1.0f,0.0f,
         0.5f, 0.5f, 0.5f, 0.0f,1.0f,0.0f, -0.5f, 0.5f, 0.5f, 0.0f,1.0f,0.0f, -0.5f,-0.5f, 0.5f, 0.0f,1.0f,0.0f,

        -0.5f, 0.5f, 0.5f, 0.0f,0.0f,1.0f, -0.5f, 0.5f,-0.5f, 0.0f,0.0f,1.0f, -0.5f,-0.5f,-0.5f, 0.0f,0.0f,1.0f,
        -0.5f,-0.5f,-0.5f, 0.0f,0.0f,1.0f, -0.5f,-0.5f, 0.5f, 0.0f,0.0f,1.0f, -0.5f, 0.5f, 0.5f, 0.0f,0.0f,1.0f,

         0.5f, 0.5f, 0.5f, 1.0f,1.0f,0.0f,  0.5f, 0.5f,-0.5f, 1.0f,1.0f,0.0f,  0.5f,-0.5f,-0.5f, 1.0f,1.0f,0.0f,
         0.5f,-0.5f,-0.5f, 1.0f,1.0f,0.0f,  0.5f,-0.5f, 0.5f, 1.0f,1.0f,0.0f,  0.5f, 0.5f, 0.5f, 1.0f,1.0f,0.0f,

        -0.5f,-0.5f,-0.5f, 1.0f,0.0f,1.0f,  0.5f,-0.5f,-0.5f, 1.0f,0.0f,1.0f,  0.5f,-0.5f, 0.5f, 1.0f,0.0f,1.0f,
         0.5f,-0.5f, 0.5f, 1.0f,0.0f,1.0f, -0.5f,-0.5f, 0.5f, 1.0f,0.0f,1.0f, -0.5f,-0.5f,-0.5f, 1.0f,0.0f,1.0f,

        -0.5f, 0.5f,-0.5f, 0.0f,1.0f,1.0f,  0.5f, 0.5f,-0.5f, 0.0f,1.0f,1.0f,  0.5f, 0.5f, 0.5f, 0.0f,1.0f,1.0f,
         0.5f, 0.5f, 0.5f, 0.0f,1.0f,1.0f, -0.5f, 0.5f, 0.5f, 0.0f,1.0f,1.0f, -0.5f, 0.5f,-0.5f, 0.0f,1.0f,1.0f
    };

    Mesh triangle(triangleVertices);
    Mesh cube(cubeVertices);

    std::unique_ptr<Shader> shader = std::make_unique<Shader>(vertexPath, fragmentPath);

    Renderer renderer;

    int second = 0;
    int interval = 165;

    glm::mat4 projection = glm::mat4(1.0f);

    float fov = glm::radians(45.0f);
    float aspectRatio = 800.0f / 600.0f;
    float near = 0.1f;
    float far = 100.0f;

    while (!glfwWindowShouldClose(window)) {
       
        renderer.begin(r, g, b);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 projection = glm::perspective(fov,aspectRatio,near,far);
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        printMatrisOnConsole(model,second,interval);

        shader->use();
        shader->setMat4("uProjection", projection);
        shader->setMat4("uView", view);
        shader->setMat4("uModel", model);

        renderer.submit(&cube, shader.get());
        renderer.end();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
void printMatrisOnConsole(glm::mat4& model,int& second,int interval)
{
    second++;
    second %= interval;

    if (second % interval == 0)
    {
        system("cls");

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                std::cout << model[i][j] << "\t";
            }
            std::cout << "\n";
        }

    }
}

