#ifndef SM_ECLIPSE_H
#define SM_ECLIPSE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Headers/hmodels/Eclipse.hpp"

// Variables globales (deberían ser extern si se usan en otro lado)
inline int stateEclipse = 0;
inline float advanceCount = 0.0;
inline float rotCount = 0.0;
inline int numberAdvance = 0;
inline int maxAdvance = 0;
constexpr float avance = 0.1;
constexpr float giroEclipse = 0.5f;

inline void sm_eclipse(std::shared_ptr<Eclipse> eclipse){
    glm::mat4 modelMatrixEclipse = glm::mat4(eclipse->getModelMatrix());
	switch (stateEclipse){
	case 0:
		if(numberAdvance == 0)
			maxAdvance = 65.0;
		else if(numberAdvance == 1)
			maxAdvance = 49.0;
		else if(numberAdvance == 2)
			maxAdvance = 44.5;
		else if(numberAdvance == 3)
			maxAdvance = 49.0;
		else if(numberAdvance == 4)
			maxAdvance = 44.5;
		stateEclipse = 1;
		break;
	case 1:
		modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f, 0.0f, avance));
		advanceCount += avance;
		eclipse->getJoints()[0] += 0.05;
		eclipse->getJoints()[1] -= 0.02;
		if(eclipse->getJoints()[1] < 0)
			eclipse->getJoints()[1] = 0;
		if(advanceCount > maxAdvance){
			advanceCount = 0;
			numberAdvance++;
			stateEclipse = 2;
		}
		break;
	case 2:
		modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0, 0.0, 0.025f));
		modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(giroEclipse), glm::vec3(0, 1, 0));
		rotCount += giroEclipse;
		eclipse->getJoints()[0] += 0.05;
		eclipse->getJoints()[1] += 0.02;
		if(eclipse->getJoints()[1] > 0.25)
			eclipse->getJoints()[1] = 0.25;
		if(rotCount >= 90.0f){
			rotCount = 0;
			stateEclipse = 0;
			if(numberAdvance > 4)
				numberAdvance = 1;
		}
		break;
	
	default:
		break;
	}
	eclipse->getModelMatrix() = modelMatrixEclipse;
}

#endif // SM_ECLIPSE_H