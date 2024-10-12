#include <GLFW/glfw3.h>
#include "Headers/GLFWInputManager.hpp"

void GLFWInputManager::keyPressed(int code, float deltaTime, int state) {
	keysState[code] = state;
}

void GLFWInputManager::mouseMoved(float mouseX, float mouseY) {
	dx = mouseX - lastMousePos.x;
	dy = lastMousePos.y - mouseY;

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
	if(mouseButtomsState[GLFW_KEY_W] == GLFW_PRESS)
		fpsCamera.moveFrontCamera(true, deltaTime);
}