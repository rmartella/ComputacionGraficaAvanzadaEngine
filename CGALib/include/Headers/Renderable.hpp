#pragma once

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
#include "Shader.h"
#include <glm/glm.hpp>

class DLL_PUBLIC Renderable {

public:
	Renderable(Shader * shader_ptr) : shader_ptr(shader_ptr) {};
	~Renderable() = default;

	void virtual render(glm::mat4 parentTrans = glm::mat4(1.0f)) = 0;

	void enableWireMode() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void disableWireMode() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

protected:
	Shader * shader_ptr;
};
