#ifndef SM_HELI_H
#define SM_HELI_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Headers/hmodels/Heli.hpp"

// Variables globales (deberían ser extern si se usan en otro lado)
int stateHeli = 0;

inline void sm_heli(std::shared_ptr<Heli> heli){
    glm::mat4 modelMatrix = glm::mat4(heli->getModelMatrix());
	switch (stateHeli)
	{
	case 0:
		heli->getJoints()[0] += 0.5;
		break;
	default:
		break;
	}
		
	heli->getModelMatrix() = modelMatrix;
}

#endif // SM_HELI_H