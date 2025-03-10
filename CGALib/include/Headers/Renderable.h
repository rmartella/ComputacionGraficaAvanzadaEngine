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

#include "AbstractModel.h"
#include "Drawable.h"

class DLL_PUBLIC Renderable: public AbstractModel, protected Drawable {

public:
    Renderable() = default;
	Renderable(Shader * shader_ptr) : shader_ptr(shader_ptr) {};
	~Renderable() = default;

	virtual void render() {
        Drawable::draw();
    }

    void enableWireMode() {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    wiredMode = true;
    }

	void disableWireMode() {
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        wiredMode = false;
	}

    void setShader(Shader * shader_ptr) { this->shader_ptr = shader_ptr; }

    Shader* getShader() { return this->shader_ptr; }

protected:
	Shader * shader_ptr;
    bool wiredMode = false;
};
