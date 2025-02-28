#ifndef _Scene_H
#define _Scene_H

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

#include <optional>

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/GLFWManager.h"
#include "Lighting.h"

class DLL_PUBLIC Scene {
public:
    void init(int width, int height, std::string strTitle, bool bFullScreen);
    virtual void run();

protected:
    virtual void render(bool render = true);
    virtual void renderSolidScene();
    virtual void renderAlphaScene(bool render = true);
    virtual void prepareScene();
    virtual void prepareDepthScene();

private:
    glm::mat4 projection;
    glm::mat4 view;
    LightManager lightManager;
    std::map<std::string, glm::vec3> blendingUnsorted;
    GLFWManager glfwManager;
};

#endif