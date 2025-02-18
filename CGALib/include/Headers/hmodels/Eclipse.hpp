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

#include "Headers/HierarchicalModel.hpp"

class DLL_PUBLIC Eclipse : public HierarchicalModel {
public:
    Eclipse(Shader* shader_ptr, BaseTerrain* terrain = nullptr): HierarchicalModel(shader_ptr, terrain){
        modelEclipseChasis = std::make_shared<Model>(shader_ptr, "../models/Eclipse/2003eclipse_chasis.obj");
        modelEclipseRearWheels = std::make_shared<Model>(shader_ptr, "../models/Eclipse/2003eclipse_rear_wheels.obj");
        modelEclipseFrontalWheels = std::make_shared<Model>(shader_ptr, "../models/Eclipse/2003eclipse_frontal_wheels.obj");
        joints.resize(2);
    }

    void render(glm::mat4 parentTrans = glm::mat4(1.0f)) {
        glm::mat4 modelMatrixEclipseChasis = glm::mat4(parentTrans * modelMatrix);
        if(terrain != nullptr)
		    modelMatrixEclipseChasis[3][1] = terrain->getHeightTerrain(modelMatrixEclipseChasis[3][0], modelMatrixEclipseChasis[3][2]);
        modelMatrixEclipseChasis = glm::scale(modelMatrixEclipseChasis, glm::vec3(0.5, 0.5, 0.5));
        modelEclipseChasis->render(modelMatrixEclipseChasis);

        glm::mat4 modelMatrixFrontalWheels = glm::mat4(modelMatrixEclipseChasis);
        modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, 1.05813, 4.11483 ));
        modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, joints[1], glm::vec3(0, 1, 0));
        modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, joints[0], glm::vec3(1, 0, 0));
        modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, -1.05813, -4.11483));
        modelEclipseFrontalWheels->render(modelMatrixFrontalWheels);

        glm::mat4 modelMatrixRearWheels = glm::mat4(modelMatrixEclipseChasis);
        modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, 1.05813, -4.35157 ));
        modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, joints[0], glm::vec3(1, 0, 0));
        modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, -1.05813, 4.35157));
        modelEclipseRearWheels->render(modelMatrixRearWheels);
    }

private:
    std::shared_ptr<Model> modelEclipseChasis;
    std::shared_ptr<Model> modelEclipseRearWheels;
    std::shared_ptr<Model> modelEclipseFrontalWheels;
};

#endif /* ECLIPSE_H_ */