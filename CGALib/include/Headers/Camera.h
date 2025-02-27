#ifndef CAMERA_H
#define CAMERA_H

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

#define SPEED 3.0f
#define SENSITIVTY 70.0f

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

class DLL_PUBLIC Camera
{
public:
	virtual void mouseMoveCamera(float xoffset, float yoffset, float dt) = 0;
	virtual void updateCamera() = 0;

	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, worldUp);
	}

	void setPosition(glm::vec3 position) {
		this->position = position;
	}

	glm::vec3 getPosition() {
		return this->position;
	}

	float getSensitivity() {
		return this->sensitivity;
	}

	void setSensitivity(float sensitivity) {
		this->sensitivity = sensitivity;
	}

	float getSpeed() {
		return this->speed;
	}

	void setSpeed(float speed) {
		this->speed = speed;
	}

protected:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw;
	float pitch;
	float sensitivity;
	float speed;
};

#endif // CAMERA_H
