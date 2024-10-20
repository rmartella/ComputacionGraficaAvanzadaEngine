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

#include "InputManager.hpp"
#include "FirstPersonCamera.h"

class GLFWInputManager : public InputManager {
public:

	GLFWInputManager(){
		camera = std::make_shared<FirstPersonCamera>();
		cameraFPS = std::dynamic_pointer_cast<FirstPersonCamera>(camera);
	}

	void keyPressed(int code, float deltaTime, int state) override;
	void mouseMoved(float mouseX, float mouseY) override;
	void mouseClicked(int code, int state) override;
	void mouseScroll(float yoffset) override;
	void controller(double deltaTime) override;

private:
	std::shared_ptr<FirstPersonCamera> cameraFPS;
};