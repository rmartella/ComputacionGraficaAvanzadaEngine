#ifndef SM_LAMBO_H
#define SM_LAMBO_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Headers/hmodels/Lambo.hpp"

// Variables globales (deberían ser extern si se usan en otro lado)
int stateLambo = 0;

inline void sm_lambo(std::shared_ptr<Lambo> lambo){
    glm::mat4 modelMatrix = glm::mat4(lambo->getModelMatrix());
	switch (stateLambo)
	{
	case 0:
		lambo->getJoints()[0] += 0.5;
		if(lambo->getJoints()[0] > 75)
			stateLambo = 1;
		break;
	case 1:
		lambo->getJoints()[0] -= 0.5;
		if(lambo->getJoints()[0] < 0){
			lambo->getJoints()[0] = 0.0;
			stateLambo = 0;
		}
	
	default:
		break;
	}
		
	lambo->getModelMatrix() = modelMatrix;
}

#endif // SM_LAMBO_H