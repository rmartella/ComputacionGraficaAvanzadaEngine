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
	if(std::dynamic_pointer_cast<ThirdPersonCamera>(camera)){
		auto tpCamera = std::dynamic_pointer_cast<ThirdPersonCamera>(camera);
		tpCamera->setDistanceFromTarget(ds);
	}
	
}

void GLFWInputManager::controller(double deltaTime) {
	// Comprobamos si es de tipo FirstPersonCamera
    if (std::dynamic_pointer_cast<FirstPersonCamera>(camera)) {
		auto fpCamera = std::dynamic_pointer_cast<FirstPersonCamera>(camera);
		if(keysState[GLFW_KEY_W] == GLFW_PRESS)
			fpCamera->moveFrontCamera(true, deltaTime);
		if (keysState[GLFW_KEY_S] == GLFW_PRESS)
			fpCamera->moveFrontCamera(false, deltaTime);
		if (keysState[GLFW_KEY_A] == GLFW_PRESS)
			fpCamera->moveRightCamera(false, deltaTime);
		if (keysState[GLFW_KEY_D] == GLFW_PRESS)
			fpCamera->moveRightCamera(true, deltaTime);
		if (mouseButtomsState[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS)
			fpCamera->mouseMoveCamera(dx, dy, deltaTime);
    }
	else if(std::dynamic_pointer_cast<ThirdPersonCamera>(camera)){
		if(mouseButtomsState[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS)
			camera->mouseMoveCamera(dx, 0.0, deltaTime);
		if(mouseButtomsState[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS)
			camera->mouseMoveCamera(0.0, dy, deltaTime);
		camera->updateCamera();
	}

	if (enableCountSelected && keysState[GLFW_KEY_TAB] == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 4)
			modelSelected = 1;
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if(keysState[GLFW_KEY_TAB] == GLFW_RELEASE)
		enableCountSelected = true;

	dx = 0;
	dy = 0;
}