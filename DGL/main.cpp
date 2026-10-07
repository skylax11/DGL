#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Mesh.h"
#include "Renderer.h"
#include "PhysicObject.h"
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>
#include <cstdlib>
#include "CameraController.h"
#include "MouseSettings.h"
#include <algorithm>

const char* vertexPath = "shaders/basic.vert";
const char* fragmentPath = "shaders/basic.frag";

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void handleMovement(GLFWwindow* window, glm::vec3& cubePos, float& rotateDirection);
void handleCollisions(std::vector<PhysicObject*>& collisionList);

glm::mat4 handleModelTransforms(const PhysicObject& object,float rotateDirection);
glm::mat4 handleViewTransforms(const glm::vec3& camPos);

void printMatrisOnConsole(glm::mat4& model, int& second, int interval);

void applyGravity(const std::vector<PhysicObject*>& objectList);
void integratePositions(const std::vector<PhysicObject*>& objectList);

std::unique_ptr<CameraController> mouseCt;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const glm::vec3 gravity(0,-9.81f,0);

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
    glm::vec3(0.0f, 4.5f,  0.0f),
    glm::vec3(0.0f, 2.5f,  0.0f),
    glm::vec3(0.0f, 0.5f,  0.0f),
    glm::vec3(0.0f, 0.0f,  0.0f),
    glm::vec3(1.5f, 0.0f,  0.0f),
    glm::vec3(-0.5f, 0.0f,  0.0f),
    glm::vec3(0.0f,-0.5f,  0.0f)
    };

    std::vector<PhysicObject*> cubeList (cubePositions.size());

    for (int i = 0; i < cubeList.size(); i++)
    {
        cubeList[i] = new PhysicObject(cubeVertices,cubePositions[i],glm::vec3 (0,1,0), glm::vec3(1, 1, 1), 0.0f, false);
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

    std::unique_ptr<PhysicObject> ground = std::make_unique<PhysicObject>(
        cubeVertices,              
        glm::vec3(0.0f, -3.0f, 0.0f),  
        glm::vec3(0.0f, 0.0f, 0.0f),   
        glm::vec3(20.0f, 1.0f, 20.0f), 
        0.0f,                 
        true                  
    );

    cubeList.push_back(ground.get());

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        deltaTime = std::min(deltaTime, 1.0f / 30.0f);

        renderer.begin(r, g, b);

        handleMovement(window,camPos,rotateDirection);

        applyGravity(cubeList);
        integratePositions(cubeList);

        handleCollisions(cubeList);

        shader->use();
        shader->setMat4("uProjection", projection);

        glm::mat4 view = handleViewTransforms(camPos);
        shader->setMat4("uView", view);

        for (int i = 0; i < cubeList.size(); i++)
        {
            glm::mat4 model = handleModelTransforms(*cubeList[i],rotateDirection);

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
void fixCollisions(std::vector<PhysicObject*>& objectList)
{
    std::vector<PhysicObject*>& c = objectList;

    for (int i = 0; i < c.size(); i++)
    {
        PhysicObject& a = *(c[i]);

        for (int j = i + 1; j < c.size(); j++)
        {
            PhysicObject& b = *(c[j]);

            if (a.isStatic && b.isStatic)
            {
                continue;
            }

            if (!a.collidesWith(b))
            {
                continue;
            }

            auto [aMin, aMax] = a.getAABB();
            auto [bMin, bMax] = b.getAABB();

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

            if (a.isStatic)
            {
                b.position -= push;           
            }
            else if (b.isStatic)
            {
                a.position += push;           
            }
            else
            {
                a.position += push * 0.5f;    
                b.position -= push * 0.5f;
            }

            a.resolveVelocity(push);          
            b.resolveVelocity(-push);         
        }
    }
}
void integratePositions(const std::vector<PhysicObject*>& objectList)
{
    for (int i = 0; i < objectList.size();i++)
    {
        PhysicObject& obj = *objectList[i];

        obj.integratePosition(deltaTime);
    }
}
void applyGravity(const std::vector<PhysicObject*>& objectList)
{
    for (int i = 0; i < objectList.size();i++)
    {
        PhysicObject& obj = *objectList[i];

        if (obj.isStatic)
        {
            continue;
        }

        obj.addVelo(gravity * deltaTime);
    }
}
void handleCollisions(std::vector<PhysicObject*>& objectList)
{
    for (int k = 0; k < 6; k++)
    {
        fixCollisions(objectList);
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
glm::mat4 handleModelTransforms(const PhysicObject& object,float rotateDirection)
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, object.position);

    if (glm::length(object.rotation) > 0.0f)
    {
        model = glm::rotate(model, (float)glfwGetTime(), object.rotation * rotateDirection);
    }    
    
    model = glm::scale(model, object.scale);

    return model;
}
glm::mat4 handleViewTransforms(const glm::vec3& camPos)
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

