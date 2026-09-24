
#include <GLFW/glfw3.h>
#include "MouseSettings.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#pragma once
class CameraController
{
public:

	float yaw = -90.0f;
	float pitch = 0.0f;
	float lastX = 400, lastY = 300;   // pencere ortasý
	bool firstMouse = true;

	glm::vec3 cameraFront = glm::vec3(0, 0, -1);
	glm::vec3 right = glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1, 0)));

	MouseSettings* m_settings;

	CameraController(GLFWwindow* window, void (*fn)(GLFWwindow* window, double xpos, double ypos), MouseSettings* mouseSettings);
	void SetMouseProperties(float _sensitivity, float _maxPitch, float _minPitch);

	~CameraController();
private:

	float delta;
	float sensitivity;
	float minPitch;
	float maxPitch;
};

