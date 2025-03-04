#ifndef KEYFRAMEANIMATOR_H_
#define KEYFRAMEANIMATOR_H_

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

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Animator.h"
#include "HierarchicalModel.h"

class DLL_PUBLIC KeyFrameAnimator : public Animator {

    public:
        KeyFrameAnimator(std::string animation, HierarchicalModel* model) : 
            animation(animation), model(model), currentJoints(model->getJoints()), currentMatrix(model->getModelMatrix()) {
            getKeyFrames(animation, keyFramesJoints, keyFramesMatrix);
        }

        void animate(){
            if(!recording){
                if(keyFramesJoints.size() > 0 && keyFramesMatrix.size() > 0){
                    interpolation = numPasos / (float) maxNumPasos;
                    numPasos++;
                    if(interpolation > 1.0){
                        numPasos = 0;
                        interpolation = 0;
                        indexFrame = indexFrameNext;
                        indexFrameNext++;
                    }
                    if(indexFrameNext > keyFramesMatrix.size() - 1)
                        indexFrameNext = 0;
                    for(u_int i = 0; i < currentJoints.size(); i++){
                        currentJoints[i] = interpolate(keyFramesJoints, indexFrame, indexFrameNext, i, interpolation);
                    }
                    currentMatrix = interpolate(keyFramesMatrix, indexFrame, indexFrameNext, 0, interpolation);
                }
            }
            model->getModelMatrix() = currentMatrix;
            model->getJoints() = currentJoints;
        }

        void record(){
            if(recording){
                appendFrame(keyFrameFile, currentJoints, currentMatrix);
            }
        }

        void setRecording(bool recording) { 
            if(!this->recording && recording){
                if(keyFrameFile.is_open())
                    keyFrameFile.close();
                keyFrameFile.open(animation);
            }
            else if(!this->recording && !recording){
                if(keyFrameFile.is_open()){
                    keyFramesJoints.clear();
                    keyFramesMatrix.clear();
                    getKeyFrames(animation, keyFramesJoints, keyFramesMatrix);
                    indexFrame = 0;
                    indexFrameNext = 1;
                    keyFrameFile.close();
                }
            }
            this->recording = recording; 
        }

        std::vector<float>& getCurrentJoints() { return currentJoints; }
        glm::mat4& getCurrentMatrix() { return currentMatrix; }

    private:
        HierarchicalModel* model;
        std::vector<float> currentJoints;
        glm::mat4 currentMatrix = glm::mat4(1.0f);
        std::vector<std::vector<float>> keyFramesJoints;
        std::vector<std::vector<glm::mat4>> keyFramesMatrix;
        std::string animation;
        bool recording = false;
        std::ofstream keyFrameFile;

        int indexFrame = 0;
        int indexFrameNext = 1;
        float interpolation = 0.0;
        int maxNumPasos = 40;
        int numPasos = 0;

        void getKeyFrames(std::string fileName, 
            std::vector<std::vector<float>> &keyFramesJoints, std::vector<std::vector<glm::mat4>> &keyFramesMatrix) {
            std::string line;
            std::ifstream infile(fileName);
            std::string s = line;
            while (std::getline(infile, line))
            {
                std::vector<glm::mat4> transforms;
                std::vector<float> rotations;
                s = line;
                size_t pos1 = 0;
                std::string token1;
                std::string delimiter1 = "|";
                while ((pos1 = s.find(delimiter1)) != std::string::npos) {
                    token1 = s.substr(0, pos1);
                    //std::cout << token1 << std::endl;

                    size_t pos2 = 0;
                    std::string token2;
                    std::string delimiter2 = ",";

                    int i = 0;
                    int j = 0;
                    glm::mat4 transform;
                    float aaa[16];
                    while ((pos2 = token1.find(delimiter2)) != std::string::npos) {
                        token2 = token1.substr(0, pos2);
                        aaa[i++] = atof(token2.c_str());
                        token1.erase(0, pos2 + delimiter2.length());
                    }
                    if(i == 0)
                        rotations.push_back(atof(token1.c_str()));
                    else{
                        aaa[i++] = atof(token1.c_str());
                        transform = glm::make_mat4(aaa);
                        transforms.push_back(transform);
                    }
                    s.erase(0, pos1 + delimiter1.length());
                }
                keyFramesMatrix.push_back(transforms);
                keyFramesJoints.push_back(rotations);
            }
        }

        float interpolate(std::vector<std::vector<float>> keyFrames, int index,
                int indexNext, int jointID, float interpolation){
            return (float)(1.0 - interpolation) * keyFrames[index][jointID] + keyFrames[indexNext][jointID] * interpolation;
        }

        glm::mat4 interpolate(std::vector<std::vector<glm::mat4>> keyFrames, int index,
                int indexNext, int jointID, float interpolation){
            glm::quat firstQuat;
            glm::quat secondQuat;
            glm::quat finalQuat;
            glm::mat4 interpoltaedMatrix;
            glm::vec4 transformComp1;
            glm::vec4 transformComp2;
            glm::vec4 finalTrans;

            firstQuat = glm::quat_cast(keyFrames[index][jointID]);
            secondQuat = glm::quat_cast(keyFrames[indexNext][jointID]);
            finalQuat = glm::slerp(firstQuat, secondQuat, interpolation);
            interpoltaedMatrix = glm::mat4_cast(finalQuat);
            transformComp1 = keyFrames[index][jointID] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            transformComp2 = keyFrames[indexNext][jointID] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            finalTrans = (float)(1.0 - interpolation) * transformComp1 + transformComp2 * interpolation;
            interpoltaedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(finalTrans)) * interpoltaedMatrix;
            return interpoltaedMatrix;
        }

        void appendFrame(std::ofstream &outputFile,
                std::vector<float> jointsList, glm::mat4 matrix){
            std::stringstream ss;
            for(unsigned int i = 0; i < jointsList.size(); i++)
                ss << jointsList[i] << "|";
            ss << matToString(matrix) << "|";
            outputFile << ss.str() << std::endl;
            //outputFile << ss.str() << "|" << std::endl;
        }

        std::string matToString(glm::mat4 matrix) {
            std::stringstream ss;
            const float *pSource = (const float*)glm::value_ptr(matrix);
            for (int i = 0; i < 16; ++i) {
                ss << pSource[i];
                if(i < 15)
                    ss << ",";
            }
            return ss.str();
        }
};

#endif /* KEYFRAMEANIMATOR_H_ */