#ifndef LAMBO_H_
#define LAMBO_H_

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

#include "Headers/Model.hpp"
#include "Headers/HierarchicalModel.hpp"

class DLL_PUBLIC Lambo : public HierarchicalModel {
public:
    Lambo(Shader* shader_ptr): HierarchicalModel(shader_ptr){
        modelLambo = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_chasis.obj");
        modelLamboLeftDor = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_left_dor.obj");
        modelLamboRightDor = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_right_dor.obj");
        modelLamboFrontLeftWheel = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_left_wheel.obj");
        modelLamboFrontRightWheel = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_right_wheel.obj");
        modelLamboRearLeftWheel = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_left_wheel.obj");
        modelLamboRearRightWheel = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_right_wheel.obj");
        joints.resize(1);
    }

    void render(glm::mat4 parentTrans = glm::mat4(1.0f)) {
        glm::mat4 modelMatrixLamboBlend = glm::mat4(parentTrans * modelMatrix);
        if(terrain != nullptr)
		    modelMatrixLamboBlend[3][1] = terrain->getHeightTerrain(modelMatrixLamboBlend[3][0], modelMatrixLamboBlend[3][2]);
        modelMatrixLamboBlend = glm::scale(modelMatrixLamboBlend, glm::vec3(1.3, 1.3, 1.3));
        modelLambo->render(modelMatrixLamboBlend);
        glActiveTexture(GL_TEXTURE0);
        glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboBlend);
        modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.08676, 0.707316, 0.982601));
        modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(joints[0]), glm::vec3(1.0, 0, 0));
        modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.08676, -0.707316, -0.982601));
        modelLamboLeftDor->render(modelMatrixLamboLeftDor);
        modelLamboRightDor->render(modelMatrixLamboBlend);
        modelLamboFrontLeftWheel->render(modelMatrixLamboBlend);
        modelLamboFrontRightWheel->render(modelMatrixLamboBlend);
        modelLamboRearLeftWheel->render(modelMatrixLamboBlend);
        modelLamboRearRightWheel->render(modelMatrixLamboBlend);
    }

private:
    std::shared_ptr<Model> modelLambo;
    std::shared_ptr<Model> modelLamboLeftDor;
    std::shared_ptr<Model> modelLamboRightDor;
    std::shared_ptr<Model> modelLamboFrontLeftWheel;
    std::shared_ptr<Model> modelLamboFrontRightWheel;
    std::shared_ptr<Model> modelLamboRearLeftWheel;
    std::shared_ptr<Model> modelLamboRearRightWheel;
};

#endif /* LAMBO_H_ */