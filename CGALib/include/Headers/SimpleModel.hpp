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

#include "Renderable.hpp"
#include "AbstractModel.hpp"
#include "Drawable.hpp"

class DLL_PUBLIC SimpleModel : public AbstractModel, protected Drawable, public Renderable {

public:
	SimpleModel(Shader * shader_ptr) : Renderable(shader_ptr) {};
	~SimpleModel() = default;

	void render(glm::mat4 parentTrans = glm::mat4(1.0f)) override {
    shader_ptr->turnOn();
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->scale);
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), this->position);
		glm::quat oX = glm::angleAxis<float>(glm::radians(orientation.x), glm::vec3(1.0, 0.0, 0.0));
		glm::quat oY = glm::angleAxis<float>(glm::radians(orientation.y), glm::vec3(0.0, 1.0, 0.0));
		glm::quat oZ = glm::angleAxis<float>(glm::radians(orientation.z), glm::vec3(0.0, 0.0, 1.0));
		glm::quat ori = oZ * oY * oX;
		modelMatrix = parentTrans * translate * glm::mat4_cast(ori) * scale;
    this->shader_ptr->setMatrix4("model", 1, GL_FALSE, glm::value_ptr(modelMatrix));
		this->shader_ptr->setVectorFloat4("ourColor", glm::value_ptr(color));
    Drawable::draw();
    shader_ptr->turnOff();
  }
};
