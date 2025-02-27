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

#include "Headers/Model.h"
#include "Headers/HierarchicalModel.h"

class DLL_PUBLIC Lambo : public HierarchicalModel {
public:
    Lambo(Shader* shader_ptr, BaseTerrain* terrain = nullptr, TYPE_COLLIDER typeCollider = BOX): HierarchicalModel(shader_ptr, terrain, typeCollider) {
        modelLambo = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_chasis.obj");
        modelLamboLeftDor = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_left_dor.obj");
        modelLamboRightDor = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_right_dor.obj");
        modelLamboFrontLeftWheel = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_left_wheel.obj");
        modelLamboFrontRightWheel = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_right_wheel.obj");
        modelLamboRearLeftWheel = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_left_wheel.obj");
        modelLamboRearRightWheel = std::make_shared<Model>(shader_ptr, "../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_right_wheel.obj");
        joints.resize(1);
        initCollider = modelLambo->getInitCollider();
        collider = modelLambo->getCollider();
    }

    void render() {
        glDisable(GL_CULL_FACE);
        animate(modelMatrix);
        glm::mat4 modelMatrixLamboBlend = glm::scale(modelMatrix, scale);
        modelLambo->getModelMatrix() = modelMatrixLamboBlend;
        modelLambo->render();
        glActiveTexture(GL_TEXTURE0);
        glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboBlend);
        modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.08676, 0.707316, 0.982601));
        modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(joints[0]), glm::vec3(1.0, 0, 0));
        modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.08676, -0.707316, -0.982601));
        modelLamboLeftDor->getModelMatrix() = modelMatrixLamboLeftDor;
        modelLamboLeftDor->render();
        modelLamboRightDor->getModelMatrix() = modelMatrixLamboBlend;
        modelLamboRightDor->render();
        modelLamboFrontLeftWheel->getModelMatrix() = modelMatrixLamboBlend;
        modelLamboFrontLeftWheel->render();
        modelLamboFrontRightWheel->getModelMatrix() = modelMatrixLamboBlend;
        modelLamboFrontRightWheel->render();
        modelLamboRearLeftWheel->getModelMatrix() = modelMatrixLamboBlend;
        modelLamboRearLeftWheel->render();
        modelLamboRearRightWheel->getModelMatrix() = modelMatrixLamboBlend;
        modelLamboRearRightWheel->render();
        this->updateCollider(modelMatrixLamboBlend);
        glEnable(GL_CULL_FACE);
    }

    void setShader(Shader* shader_ptr) override { 
        Renderable::setShader(shader_ptr);
        modelLambo->setShader(shader_ptr);
        modelLamboLeftDor->setShader(shader_ptr);
        modelLamboRightDor->setShader(shader_ptr);
        modelLamboFrontLeftWheel->setShader(shader_ptr);
        modelLamboFrontRightWheel->setShader(shader_ptr);
        modelLamboRearLeftWheel->setShader(shader_ptr);
        modelLamboRearRightWheel->setShader(shader_ptr);
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