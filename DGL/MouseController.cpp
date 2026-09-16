#include "MouseController.h"
#include "MouseSettings.h"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>



MouseController::MouseController(GLFWwindow* window, void(*fn)(GLFWwindow* window, double xpos, double ypos), MouseSettings* mouseSettings)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, fn);

	m_settings = mouseSettings;
	SetMouseProperties(mouseSettings->m_sensitivity, mouseSettings->m_maxPitch, mouseSettings->m_minPitch);
}

void MouseController::SetMouseProperties(float _sensitivity, float _maxPitch, float _minPitch)
{
	sensitivity = _sensitivity;
	maxPitch = _maxPitch;
	minPitch = _minPitch;
}

MouseController::~MouseController()
{
}
