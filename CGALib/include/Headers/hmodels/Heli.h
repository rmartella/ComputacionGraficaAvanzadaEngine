#ifndef HELI_H_
#define HELI_H_

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

class DLL_PUBLIC Heli : public HierarchicalModel {
public:
    Heli(Shader* shader_ptr): HierarchicalModel(shader_ptr){
        modelHeliChasis = std::make_shared<Model>(shader_ptr, "../models/Helicopter/Mi_24_chasis.obj");
        modelHeliHeli = std::make_shared<Model>(shader_ptr, "../models/Helicopter/Mi_24_heli.obj");
        modelHeliHeliBack = std::make_shared<Model>(shader_ptr, "../models/Helicopter/Mi_24_heli_back.obj");
        joints.resize(1);
    }

    void render() {
        animate(modelMatrix);
        glm::mat4 modelMatrixHeliChasis = glm::scale(modelMatrix, scale);
        modelHeliChasis->getModelMatrix() = modelMatrixHeliChasis;
        modelHeliChasis->render();

        glm::mat4 modelMatrixHeliHeli = glm::mat4(modelMatrixHeliChasis);
        modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, -0.249548));
        modelMatrixHeliHeli = glm::rotate(modelMatrixHeliHeli, joints[0], glm::vec3(0, 1, 0));
        modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, 0.249548));
        modelHeliHeli->getModelMatrix() = modelMatrixHeliHeli;
        modelHeliHeli->render();
    }

    void setShader(Shader* shader_ptr) override { 
        Renderable::setShader(shader_ptr);
        modelHeliChasis->setShader(shader_ptr);
        modelHeliHeli->setShader(shader_ptr);
        modelHeliHeliBack->setShader(shader_ptr);
    }

private:
    std::shared_ptr<Model> modelHeliChasis;
    std::shared_ptr<Model> modelHeliHeli;
    std::shared_ptr<Model> modelHeliHeliBack;
};

#endif /* HELI_H_ */