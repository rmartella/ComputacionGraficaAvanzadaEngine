#pragma once
/*
Creado por Reynaldo Martell
Computacion Grafica e Interaccion Humano Computadora
Fecha: 08/02/2018
*/

#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

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

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

class DLL_PUBLIC AbstractModel {
public:

	AbstractModel() = default;

	~AbstractModel() = default;

	glm::vec3 getPosition() {
		return this->position;
	}

	glm::vec3 getScale() {
		return this->scale;
	}

	glm::vec3 getOrientation() {
		return this->orientation;
	}

	void setPosition(glm::vec3 position) {
		this->position = position;
		generatModelMatrix();
	}

	void setScale(glm::vec3 scale) {
		this->scale = scale;
	}

	void setOrientation(glm::vec3 orientation) {
		this->orientation = orientation;
		generatModelMatrix();
	}

	void setColor(const glm::vec4& color) {
		this->color = color;
	}

	glm::mat4& getModelMatrix(){
		return this->modelMatrix;
	}

protected:
	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 scale = glm::vec3(1.0);
	glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 1.0);
	glm::vec3 orientation = glm::vec3(0.0);
	glm::mat4 modelMatrix = glm::mat4(1.0);

	void generatModelMatrix(){
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), this->position);
		glm::quat oX = glm::angleAxis<float>(glm::radians(orientation.x), glm::vec3(1.0, 0.0, 0.0));
		glm::quat oY = glm::angleAxis<float>(glm::radians(orientation.y), glm::vec3(0.0, 1.0, 0.0));
		glm::quat oZ = glm::angleAxis<float>(glm::radians(orientation.z), glm::vec3(0.0, 0.0, 1.0));
		glm::quat ori = oZ * oY * oX;
		modelMatrix = translate * glm::mat4_cast(ori);
	}
};

#endif // ABSTRACTMODEL_H
