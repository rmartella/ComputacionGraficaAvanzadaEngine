#ifndef GLFWMANAGER_H_
#define GLFWMANAGER_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "WindowManager.hpp"
#include "TimeManager.h"

class GLFWManager: public WindowManager {
public:
	GLFWManager();
	virtual ~GLFWManager();

	virtual bool init(int width, int height, std::string strTitle,
			bool isFullScreen);
	virtual void swapTheBuffers();
	virtual bool processInput(bool continueApplication);

	static void reshapeCallback(GLFWwindow* Window, int widthRes,
			int heightRes);
	static void keyCallback(GLFWwindow* window, int key, int scancode,
			int action, int mode);
	static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int state,
			int mod);
	static void mouseScrollCallback(GLFWwindow* window, double xoffset,
			double yoffset);

	static double deltaTime;

protected:
	virtual void destroy();
	GLFWwindow * window;
};

#endif /* GLFWMANAGER_H_ */