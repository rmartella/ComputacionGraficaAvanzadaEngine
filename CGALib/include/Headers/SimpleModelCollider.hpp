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

#include "ObjectCollider.hpp"
#include "ModelBase.hpp"
#include "Drawable.hpp"

class DLL_PUBLIC SimpleModelCollider : public ModelBase, protected Drawable, public ObjectCollider{

public:
	SimpleModelCollider(Shader* shader_ptr, TYPE_COLLIDER typeCollider) : ModelBase(shader_ptr), ObjectCollider(typeCollider) {};
	~SimpleModelCollider() = default;

	void render() override {
        GLint polygonMode[2];  // Almacena los modos para GL_FRONT y GL_BACK
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if(wiredMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shader_ptr->turnOn();
        glm::mat4 finalModelMatrix = modelMatrix * glm::scale(glm::mat4(1.0), scale);
        this->shader_ptr->setMatrix4("model", 1, GL_FALSE, glm::value_ptr(finalModelMatrix));
        this->shader_ptr->setVectorFloat4("ourColor", glm::value_ptr(color));
        Drawable::draw();
        this->updateCollider();
        shader_ptr->turnOff();
        glPolygonMode(GL_FRONT, polygonMode[0]);
        glPolygonMode(GL_BACK, polygonMode[1]);
  }
};
