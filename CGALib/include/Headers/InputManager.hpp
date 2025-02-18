#ifndef _InputManager_H
#define _InputManager_H

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif

#include <map>
#include <string>
#include <memory>

#define _USE_MATH_DEFINES
#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"

class InputManager {
public:

	InputManager(std::shared_ptr<Camera> camera) : camera(camera), 
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

	std::shared_ptr<Camera> getCamera(){
		return camera;
	}

    void setCamera(std::shared_ptr<Camera> camera){
		this->camera = camera;
	}

	int& getModelSelected() { return modelSelected; }

protected:
	glm::ivec2 lastMousePos;
	std::map<int, bool> mouseButtomsState;
	std::map<int, bool> keysState;

	float dx, dy, ds;
	std::shared_ptr<Camera> camera;
	int modelSelected = 1;
};

#endif