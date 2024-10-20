#include <GLFW/glfw3.h>
#include "Headers/GLFWInputManager.hpp"

void GLFWInputManager::keyPressed(int code, float deltaTime, int state) {
	keysState[code] = state;
}

void GLFWInputManager::mouseMoved(float mouseX, float mouseY) {
	dx = mouseX - lastMousePos.x;
	dy = mouseY - lastMousePos.y;

	lastMousePos.x = mouseX;
	lastMousePos.y = mouseY;
}

void GLFWInputManager::mouseClicked(int code, int state) {
	mouseButtomsState[code] = state;
}

void GLFWInputManager::mouseScroll(float yoffset) {
    ds -= yoffset;
}

void GLFWInputManager::controller(double deltaTime) {
	if(keysState[GLFW_KEY_W] == GLFW_PRESS)
		cameraFPS->moveFrontCamera(true, deltaTime);
	if (keysState[GLFW_KEY_S] == GLFW_PRESS)
		cameraFPS->moveFrontCamera(false, deltaTime);
	if (keysState[GLFW_KEY_A] == GLFW_PRESS)
		cameraFPS->moveRightCamera(false, deltaTime);
	if (keysState[GLFW_KEY_D] == GLFW_PRESS)
		cameraFPS->moveRightCamera(true, deltaTime);
	if (mouseButtomsState[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS)
		cameraFPS->mouseMoveCamera(dx, dy, deltaTime);

	dx = 0;
	dy = 0;
}