#ifndef TERRAINANIMATE_H_
#define TERRAINANIMATE_H_

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

#include "TimeManager.h"
#include "BaseTerrain.h"

class DLL_PUBLIC TerrainAnimator {

    public:
        TerrainAnimator(BaseTerrain* terrain = nullptr) : terrain(terrain) {}
        void animate(glm::mat4& modelMatrix) {
            if(terrain != nullptr){              

                modelMatrix[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain->getHeightTerrain(modelMatrix[3][0], modelMatrix[3][2]) + offsetHeight;
                tmv = TimeManager::Instance().GetTime() - startTimeJump;
                if(modelMatrix[3][1] < terrain->getHeightTerrain(modelMatrix[3][0], modelMatrix[3][2]) + offsetHeight){
                    isJump = false;
                    modelMatrix[3][1] = terrain->getHeightTerrain(modelMatrix[3][0], modelMatrix[3][2]) + offsetHeight;
                }
                if(alignTerrain){
                    glm::vec3 ejey = glm::normalize(terrain->getNormalTerrain(modelMatrix[3][0], modelMatrix[3][2]));
                    glm::vec3 ejex = glm::vec3(modelMatrix[0]);
                    glm::vec3 ejez = glm::normalize(glm::cross(ejex, ejey));
                    ejex = glm::normalize(glm::cross(ejey, ejez));

                    modelMatrix[0] = glm::vec4(ejex, 0.0);
                    modelMatrix[1] = glm::vec4(ejey, 0.0);
                    modelMatrix[2] = glm::vec4(ejez, 0.0);
                }
            }
        }

        void startJump(){
            if(!isJump){
                this->isJump = true;
                this->startTimeJump = TimeManager::Instance().GetTime();
                this->tmv = false;
            }
        }

        void setOffsetHeight(float offsetHeight) { this->offsetHeight = offsetHeight; }
        void enaDisAlignTerrain(bool alignTerrain) { this->alignTerrain =  alignTerrain; }

    protected:
        BaseTerrain* terrain;

    private:
        bool isJump = false;
        float GRAVITY = 1.81;
        double tmv = 0;
        double startTimeJump = 0;
        float offsetHeight = 0.0;
        bool alignTerrain = false;
};

#endif /* TERRAINANIMATE_H_ */