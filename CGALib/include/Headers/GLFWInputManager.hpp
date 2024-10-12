/*#include <map>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>*/

#include "InputManager.hpp"
#include "FirstPersonCamera.h"

class GLFWInputManager : public InputManager {
public:

	GLFWInputManager() = default;

	void keyPressed(int code, float deltaTime, int state) override;
	void mouseMoved(float mouseX, float mouseY) override;
	void mouseClicked(int code, int state) override;
	void mouseScroll(float yoffset) override;
	void controller(double deltaTime) override;

	Camera& getCurrentCamera() {return fpsCamera; }

private:
	FirstPersonCamera fpsCamera;
};