#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Mesh.h"
#include "Renderer.h"
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>
#include <cstdlib>
#include "CameraController.h"
#include "MouseSettings.h"
#include <algorithm>

enum AABBCompareAxis
{
    X,
    Y,
    Z
};

const float aaBBOffset (0.25f);

struct AABB
{

public:

    AABB() : min(0.0f), max(0.0f)
    {

    }
    AABB(glm::vec3 _min, glm::vec3 _max) : min(_min - aaBBOffset), max(_max + aaBBOffset)
    {

    }
    bool collidesWith(const AABB& neighbour)
    {
        bool x = min.x <= neighbour.max.x && max.x >= neighbour.min.x;
        bool y = min.y <= neighbour.max.y && max.y >= neighbour.min.y;
        bool z = min.z <= neighbour.max.z && max.z >= neighbour.min.z;
        return x && y && z;
    }
    int getDirectionTo(const AABB& neighbour, AABBCompareAxis axis) const
    {
        float centerA, centerB;

        if (axis == AABBCompareAxis::X) {
            centerA = (min.x + max.x) / 2;
            centerB = (neighbour.min.x + neighbour.max.x) / 2;
        }
        else if (axis == AABBCompareAxis::Y) {
            centerA = (min.y + max.y) / 2;
            centerB = (neighbour.min.y + neighbour.max.y) / 2;
        }
        else {  
            centerA = (min.z + max.z) / 2;
            centerB = (neighbour.min.z + neighbour.max.z) / 2;
        }
        return (centerA < centerB) ? -1 : 1;
    }
    void setMin(glm::vec3 newMin) { min = newMin;}  
    void setMax(glm::vec3 newMax) { max = newMax;}
    const glm::vec3 getMin()
    {
        return min;
    }
    const glm::vec3 getMax()
    {
        return max;
    }

private:
    glm::vec3 min;
    glm::vec3 max;

    
};

const char* vertexPath = "shaders/basic.vert";
const char* fragmentPath = "shaders/basic.frag";

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void handleMovement(GLFWwindow* window, glm::vec3& cubePos, float& rotateDirection);
void handleCollisions(GLFWwindow* window, std::vector<glm::vec3>* cubePositions, std::vector<AABB>* collisionList);

glm::mat4 handleModelTransforms(glm::vec3& basePosition,float rotateDirection);
glm::mat4 handleViewTransforms(glm::vec3& camPos);

void printMatrisOnConsole(glm::mat4& model, int& second, int interval);

std::unique_ptr<CameraController> mouseCt;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

    Mesh cube(cubeVertices);

    std::vector<glm::vec3> cubePositions = {
    glm::vec3(0.0f, 0.0f,  0.0f),
    glm::vec3(1.5f, 0.0f,  0.0f),
    glm::vec3(-0.5f, 0.0f,  0.0f),
    glm::vec3(0.0f, 0.5f,  0.0f),
    glm::vec3(0.0f,-0.5f,  0.0f)
    };

    std::vector<AABB> cubeCollisions (cubePositions.size());

    for (int i = 0; i < cubeCollisions.size(); i++)
    {
        glm::vec3 position = cubePositions[i];

        glm::vec3 minPoint(FLT_MAX);
        glm::vec3 maxPoint(-FLT_MAX);

        for (int j = 0; j < cubeVertices.size(); j+= 6)
        {
            float x = cubeVertices[j];
            float y = cubeVertices[j+1];
            float z = cubeVertices[j+2];

            minPoint.x = std::min(minPoint.x, x + position.x);
            minPoint.y = std::min(minPoint.y, y + position.y);
            minPoint.z = std::min(minPoint.z, z + position.z);

            maxPoint.x = std::max(maxPoint.x, x + position.x);
            maxPoint.y = std::max(maxPoint.y, y + position.y);
            maxPoint.z = std::max(maxPoint.z, z + position.z);
        }

        cubeCollisions[i] = AABB(minPoint, maxPoint);
    }

    std::unique_ptr<Shader> shader = std::make_unique<Shader>(vertexPath, fragmentPath);

    Renderer renderer;

    int second = 0;
    int interval = 165;

    float fov = glm::radians(45.0f);
    float aspectRatio = 800.0f / 600.0f;
    float near = 0.1f;
    float far = 100.0f;

    glm::vec3 camPos(0, 0, 5);
    float rotateDirection (1.0f);
    glm::mat4 projection = glm::perspective(fov, aspectRatio, near, far);

    MouseSettings mouseSettings (0.1f,90.0f,-90.0f);

    
    CameraController* mouseCtr = new CameraController(window, mouse_callback, &mouseSettings);
    mouseCt = std::unique_ptr<CameraController>(mouseCtr);


    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
       
        renderer.begin(r, g, b);

        handleMovement(window,camPos,rotateDirection);
        handleCollisions(window,&cubePositions,&cubeCollisions);

        shader->use();
        shader->setMat4("uProjection", projection);

        glm::mat4 view = handleViewTransforms(camPos);
        shader->setMat4("uView", view);

        for (int i = 0; i < cubePositions.size(); i++)
        {
            glm::mat4 model = handleModelTransforms(cubePositions[i],rotateDirection);

            //printMatrisOnConsole(model, second, interval);

            shader->setMat4("uModel", model);

            renderer.submit(&cube, shader.get(),model);
        }

        renderer.end();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
void fixCollisions(std::vector<glm::vec3>* cubePositions, std::vector<AABB>* collisionList)
{
    std::vector<AABB>& c = (*collisionList);

    for (int i = 0; i < c.size(); i++)
    {
        AABB& a = c[i];

        for (int j = i + 1; j < c.size(); j++)
        {
            AABB& b = c[j];

            if (a.collidesWith(b))
            {
                glm::vec3 aMax = a.getMax();
                glm::vec3 aMin = a.getMin();
                glm::vec3 bMax = b.getMax();
                glm::vec3 bMin = b.getMin();

                float overlapX = std::min(aMax.x, bMax.x) - std::max(aMin.x, bMin.x);
                float overlapY = std::min(aMax.y, bMax.y) - std::max(aMin.y, bMin.y);
                float overlapZ = std::min(aMax.z, bMax.z) - std::max(aMin.z, bMin.z);

                glm::vec3 push(0.0f);
                if (overlapX <= overlapY && overlapX <= overlapZ)
                    push.x = overlapX * a.getDirectionTo(b, AABBCompareAxis::X);
                else if (overlapY <= overlapZ)
                    push.y = overlapY * a.getDirectionTo(b, AABBCompareAxis::Y);
                else
                    push.z = overlapZ * a.getDirectionTo(b, AABBCompareAxis::Z);

                glm::vec3 half = push * 0.5f;

                (*cubePositions)[i] += half;
                a.setMin(aMin + half);
                a.setMax(aMax + half);

                (*cubePositions)[j] -= half;
                b.setMin(bMin - half);
                b.setMax(bMax - half);
            }
        }
    }
}
void handleCollisions(GLFWwindow* window, std::vector<glm::vec3>* cubePositions, std::vector<AABB>* collisionList)
{
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        fixCollisions(cubePositions,collisionList);
    }
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (mouseCt->firstMouse)
    {
        mouseCt->lastX = xpos;
        mouseCt->lastY = ypos;
        mouseCt->firstMouse = false;
    }

    float xOffset = xpos - mouseCt->lastX;
    float yOffset = mouseCt->lastY - ypos;

    mouseCt->lastX = xpos;
    mouseCt->lastY = ypos;

    MouseSettings settings = *(mouseCt->m_settings);
    float sensitivity = settings.m_sensitivity;

    xOffset *= sensitivity;
    yOffset *= sensitivity;

    mouseCt->yaw += xOffset;
    mouseCt->pitch += yOffset;

    mouseCt->pitch = std::clamp(mouseCt->pitch, settings.m_minPitch, settings.m_maxPitch);

    glm::vec3 direction;

    float xzLen = glm::cos(glm::radians(mouseCt->pitch));

    direction.x = xzLen * cos(glm::radians(mouseCt->yaw));
    direction.y = glm::sin(glm::radians(mouseCt->pitch));
    direction.z = xzLen * glm::sin(glm::radians(mouseCt->yaw));

    mouseCt->cameraFront = glm::normalize(direction);
    mouseCt->right = glm::normalize(glm::cross(mouseCt->cameraFront, glm::vec3(0, 1, 0)));
}
void handleMovement(GLFWwindow* window, glm::vec3& camPos,float& rotateDirection)
{
    float speed = 1.0f;

    glm::vec3 front = mouseCt->cameraFront;
    front.y = 0;
    front = glm::normalize(front);

    glm::vec3 right = mouseCt->right;
    right.y = 0;
    right = glm::normalize(right);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camPos += front * speed * deltaTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camPos -= front * speed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camPos += right * speed * deltaTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camPos -= right * speed * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        rotateDirection = 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        rotateDirection = -1;
    }
}
glm::mat4 handleModelTransforms(glm::vec3& basePosition,float rotateDirection)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, basePosition);
    //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f) * rotateDirection);

    return model;
}
glm::mat4 handleViewTransforms(glm::vec3& camPos)
{
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(camPos, camPos + mouseCt->cameraFront, glm::vec3(0, 1, 0));

    return view;
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

