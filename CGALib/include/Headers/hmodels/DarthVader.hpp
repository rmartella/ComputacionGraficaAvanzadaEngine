#ifndef DARTHVADER_H_
#define DARTHVADER_H_

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

class DLL_PUBLIC DarthVader : public HierarchicalModel
{
public:
    DarthVader(Shader *shader_ptr, BaseTerrain* terrain = nullptr) : HierarchicalModel(shader_ptr, terrain)
    {
        modelDartLegoBody = std::make_shared<Model>(shader_ptr, "../models/LegoDart/LeoDart_body.obj");
        modelDartLegoHead = std::make_shared<Model>(shader_ptr, "../models/LegoDart/LeoDart_head.obj");
        modelDartLegoMask = std::make_shared<Model>(shader_ptr, "../models/LegoDart/LeoDart_mask.obj");
        modelDartLegoLeftArm = std::make_shared<Model>(shader_ptr, "../models/LegoDart/LeoDart_left_arm.obj");
        modelDartLegoRightArm = std::make_shared<Model>(shader_ptr, "../models/LegoDart/LeoDart_right_arm.obj");
        modelDartLegoLeftHand = std::make_shared<Model>(shader_ptr, "../models/LegoDart/LeoDart_left_hand.obj");
        modelDartLegoRightHand = std::make_shared<Model>(shader_ptr, "../models/LegoDart/LeoDart_right_hand.obj");
        modelDartLegoLeftLeg = std::make_shared<Model>(shader_ptr, "../models/LegoDart/LeoDart_left_leg.obj");
        modelDartLegoRightLeg = std::make_shared<Model>(shader_ptr, "../models/LegoDart/LeoDart_right_leg.obj");
        joints.resize(7);
        collider = modelDartLegoBody->getCollider();
        initCollider = modelDartLegoBody->getInitCollider();
    }

    void render()
    {
        animate(modelMatrix);
        // Se deshabilita el cull faces IMPORTANTE para la capa
        glDisable(GL_CULL_FACE);
        glm::mat4 modelMatrixDartBody = glm::scale(modelMatrix, glm::vec3(scale));
        modelDartLegoBody->getModelMatrix() = modelMatrixDartBody;
        modelDartLegoBody->render();
        glm::mat4 modelMatrixDartHead = glm::mat4(modelMatrixDartBody);
        modelMatrixDartHead = glm::rotate(modelMatrixDartHead, joints[0], glm::vec3(0, 1, 0));
        modelDartLegoHead->getModelMatrix() = modelMatrixDartHead;
        modelDartLegoHead->render();
        modelDartLegoMask->getModelMatrix() = modelMatrixDartHead;
        modelDartLegoMask->render();
        glm::mat4 modelMatrixDartLeftArm = glm::mat4(modelMatrixDartBody);
        modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
        modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
        modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, joints[1], glm::vec3(0, 0, 1));
        modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
        modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
        modelDartLegoLeftArm->getModelMatrix() = modelMatrixDartLeftArm;
        modelDartLegoLeftArm->render();
        glm::mat4 modelMatrixDartLeftHand = glm::mat4(modelMatrixDartLeftArm);
        modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
        modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
        modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, joints[2], glm::vec3(0, 1, 0));
        modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
        modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
        modelDartLegoLeftHand->getModelMatrix() = modelMatrixDartLeftHand;
        modelDartLegoLeftHand->render();
        glm::mat4 modelMatrixDartRightArm = glm::mat4(modelMatrixDartBody);
        modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
        modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
        modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, joints[3], glm::vec3(0, 0, 1));
        modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
        modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
        modelDartLegoRightArm->getModelMatrix() = modelMatrixDartRightArm;
        modelDartLegoRightArm->render();
        glm::mat4 modelMatrixDartRightHand = glm::mat4(modelMatrixDartRightArm);
        modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
        modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
        modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, joints[4], glm::vec3(0, 1, 0));
        modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
        modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
        modelDartLegoRightHand->getModelMatrix() = modelMatrixDartRightHand;
        modelDartLegoRightHand->render();
        glm::mat4 modelMatrixDartLeftLeg = glm::mat4(modelMatrixDartBody);
        modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, 1.12632, 0.423349));
        modelMatrixDartLeftLeg = glm::rotate(modelMatrixDartLeftLeg, joints[5], glm::vec3(0, 0, 1));
        modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, -1.12632, -0.423349));
        modelDartLegoLeftLeg->getModelMatrix() = modelMatrixDartLeftLeg;
        modelDartLegoLeftLeg->render();
        glm::mat4 modelMatrixDartRightLeg = glm::mat4(modelMatrixDartBody);
        modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, 1.12632, -0.423349));
        modelMatrixDartRightLeg = glm::rotate(modelMatrixDartRightLeg, joints[6], glm::vec3(0, 0, 1));
        modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, -1.12632, 0.423349));
        modelDartLegoRightLeg->getModelMatrix() = modelMatrixDartRightLeg;
        modelDartLegoRightLeg->render();
        // Se regresa el cull faces IMPORTANTE para la capa
        glEnable(GL_CULL_FACE);
        this->updateCollider(modelMatrixDartBody);
    }

private:
    std::shared_ptr<Model> modelDartLegoBody;
    std::shared_ptr<Model> modelDartLegoHead;
    std::shared_ptr<Model> modelDartLegoMask;
    std::shared_ptr<Model> modelDartLegoLeftArm;
    std::shared_ptr<Model> modelDartLegoRightArm;
    std::shared_ptr<Model> modelDartLegoLeftHand;
    std::shared_ptr<Model> modelDartLegoRightHand;
    std::shared_ptr<Model> modelDartLegoLeftLeg;
    std::shared_ptr<Model> modelDartLegoRightLeg;
};

#endif /* DARTHVADER_H_ */