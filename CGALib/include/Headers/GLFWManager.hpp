#ifndef GLFWMANAGER_H_
#define GLFWMANAGER_H_

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