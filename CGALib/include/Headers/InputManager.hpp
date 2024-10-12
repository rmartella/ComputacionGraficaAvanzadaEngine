#ifndef _InputManager_H
#define _InputManager_H

#include <map>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

class InputManager {
public:

	InputManager() :
			lastMousePos(glm::ivec2(0.0)), dx(0), dy(0), ds(0) {}

	void virtual keyPressed(int code, float deltaTime, int state) = 0;
	void virtual mouseMoved(float mouseX, float mouseY) = 0;
	void virtual mouseClicked(int code, int state) = 0;
	void virtual mouseScroll(float yoffset) = 0;
	void virtual controller(double deltaTime) = 0;

	bool getStatusKey(int key) { 
		auto keyState = keysState.find(key);
		if(keyState != keysState.end())
			return keyState->second;
		return false;
	}

protected:
	glm::ivec2 lastMousePos;
	std::map<int, bool> mouseButtomsState;
	std::map<int, bool> keysState;

	float dx, dy, ds;
};

#endif