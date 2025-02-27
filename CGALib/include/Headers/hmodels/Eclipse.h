#ifndef ECLIPSE_H_
#define ECLIPSE_H_

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

#include "Headers/HierarchicalModel.h"

class DLL_PUBLIC Eclipse : public HierarchicalModel {
public:
    Eclipse(Shader* shader_ptr, BaseTerrain* terrain = nullptr, TYPE_COLLIDER typeCollider = BOX): HierarchicalModel(shader_ptr, terrain, typeCollider){
        modelEclipseChasis = std::make_shared<Model>(shader_ptr, "../models/Eclipse/2003eclipse_chasis.obj");
        modelEclipseRearWheels = std::make_shared<Model>(shader_ptr, "../models/Eclipse/2003eclipse_rear_wheels.obj");
        modelEclipseFrontalWheels = std::make_shared<Model>(shader_ptr, "../models/Eclipse/2003eclipse_frontal_wheels.obj");
        joints.resize(2);
        initCollider = modelEclipseChasis->getInitCollider();
        collider = modelEclipseChasis->getCollider();
    }

    void animate(glm::mat4& modelMatrix) {
        modelMatrix[3][1] = terrain->getHeightTerrain(modelMatrix[3][0], modelMatrix[3][2]);
        glm::vec4 frontalLeftWheel = glm::translate(modelMatrix, glm::vec3(2.8, 1.05813, 4.11483))[3];
		glm::vec4 frontalRightWheel = glm::translate(modelMatrix, glm::vec3(-2.8, 1.05813, 4.11483))[3];
		glm::vec4 rearLeftWheel = glm::translate(modelMatrix, glm::vec3(2.8, 1.05813, -4.35157))[3];
		glm::vec4 rearRightWheel = glm::translate(modelMatrix, glm::vec3(-2.8, 1.05813, -4.35157))[3];
		frontalLeftWheel.y = terrain->getHeightTerrain(frontalLeftWheel.x, frontalLeftWheel.z);
		frontalRightWheel.y = terrain->getHeightTerrain(frontalRightWheel.x, frontalRightWheel.z);
		rearLeftWheel.y = terrain->getHeightTerrain(rearLeftWheel.x, rearLeftWheel.z);
		rearRightWheel.y = terrain->getHeightTerrain(rearRightWheel.x, rearRightWheel.z);

		glm::vec3 uVec = rearLeftWheel - frontalLeftWheel;
    	glm::vec3 vVec = rearRightWheel - frontalLeftWheel;
		glm::vec4 ejeyEclipse = glm::vec4(glm::normalize(glm::vec3(glm::cross(glm::vec3(uVec), glm::vec3(vVec)))), 0);
		glm::vec4 ejexEclipse = modelMatrix[0];
		glm::vec4 ejezEclipse = glm::vec4(glm::normalize(glm::vec3(glm::cross(glm::vec3(ejexEclipse), glm::vec3(ejeyEclipse)))), 0);
		ejexEclipse = glm::vec4(glm::normalize(glm::vec3(glm::cross(glm::vec3(ejeyEclipse), glm::vec3(ejezEclipse)))), 0);
		modelMatrix[0] = ejexEclipse;
		modelMatrix[1] = ejeyEclipse;
		modelMatrix[2] = ejezEclipse;
    }

    void render() {
        this->animate(modelMatrix);
        glm::mat4 modelMatrixEclipseChasis = glm::scale(modelMatrix, scale);
        modelEclipseChasis->getModelMatrix() = modelMatrixEclipseChasis;
        modelEclipseChasis->render();

        glm::mat4 modelMatrixFrontalWheels = glm::mat4(modelMatrixEclipseChasis);
        modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, 1.05813, 4.11483 ));
        modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, joints[1], glm::vec3(0, 1, 0));
        modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, joints[0], glm::vec3(1, 0, 0));
        modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, -1.05813, -4.11483));
        modelEclipseFrontalWheels->getModelMatrix() = modelMatrixFrontalWheels;
        modelEclipseFrontalWheels->render();

        glm::mat4 modelMatrixRearWheels = glm::mat4(modelMatrixEclipseChasis);
        modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, 1.05813, -4.35157 ));
        modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, joints[0], glm::vec3(1, 0, 0));
        modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, -1.05813, 4.35157));
        modelEclipseRearWheels->getModelMatrix() = modelMatrixRearWheels;
        modelEclipseRearWheels->render();

        this->updateCollider(modelMatrixEclipseChasis);
    }

    void setShader(Shader* shader_ptr) override { 
        Renderable::setShader(shader_ptr);
        modelEclipseChasis->setShader(shader_ptr);
        modelEclipseFrontalWheels->setShader(shader_ptr);
        modelEclipseRearWheels->setShader(shader_ptr);
    }

private:
    std::shared_ptr<Model> modelEclipseChasis;
    std::shared_ptr<Model> modelEclipseRearWheels;
    std::shared_ptr<Model> modelEclipseFrontalWheels;
};

#endif /* ECLIPSE_H_ */