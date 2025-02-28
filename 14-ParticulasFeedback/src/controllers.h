#include "Headers/GLFWManager.h"
#include "global.h"

void darth_controller(int modelSelected){
	if(modelSelected != 1)
		return;
	if(GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) && 
		GLFWManager::inputManager->getStatusKey(GLFW_KEY_R))
		darthAnimation->setRecording(true);
	if(!GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) && 
		GLFWManager::inputManager->getStatusKey(GLFW_KEY_R))
		darthAnimation->setRecording(false);
	if(availableSave && GLFWManager::inputManager->getStatusKey(GLFW_KEY_ENTER)){
		darthAnimation->record();
		availableSave = false;
	}
	if(!GLFWManager::inputManager->getStatusKey(GLFW_KEY_ENTER))
		availableSave = true;

	// Dart Lego model movements
	if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_1) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[0] += 0.02;
	else if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_1) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[0] -= 0.02;
	if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_2) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[1] += 0.02;
	else if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_2) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[1] -= 0.02;
	if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_3) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[2] += 0.02;
	else if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_3) == GLFW_PRESS &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[2] -= 0.02;
	if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_4) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[3] += 0.02;
	else if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_4) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[3] -= 0.02;
	if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_5) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[4] += 0.02;
	else if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_5) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[4] -= 0.02;
	if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_6) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[5] += 0.02;
	else if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_6) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[5] -= 0.02;
	if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_7) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[6] += 0.02;
	else if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			GLFWManager::inputManager->getStatusKey(GLFW_KEY_7) == GLFW_PRESS)
		darthAnimation->getCurrentJoints()[6] -= 0.02;
	glm::mat4 modelMatrixDarth = darthAnimation->getCurrentMatrix();
	if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixDarth = glm::rotate(modelMatrixDarth, 0.02f, glm::vec3(0, 1, 0));
	else if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixDarth = glm::rotate(modelMatrixDarth, -0.02f, glm::vec3(0, 1, 0));
	if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixDarth = glm::translate(modelMatrixDarth, glm::vec3(-0.02, 0.0, 0.0));
	else if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixDarth = glm::translate(modelMatrixDarth, glm::vec3(0.02, 0.0, 0.0));
	darthAnimation->getCurrentMatrix() = modelMatrixDarth;
	GLFWManager::inputManager->getCamera()->updateCamera();
}

void mayow_controller(int modelSelected){
	mayowModelAnimate->setAnimationIndex(1);
	if(modelSelected != 2)
		return;
	glm::mat4 modelMatrixMayow = mayowModelAnimate->getModelMatrix();
	if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT) == GLFW_PRESS){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, 0.02f, glm::vec3(0, 1, 0));
		mayowModelAnimate->setAnimationIndex(0);
	}
	else if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_RIGHT) == GLFW_PRESS){
		modelMatrixMayow = glm::rotate(modelMatrixMayow, -0.02f, glm::vec3(0, 1, 0));
		mayowModelAnimate->setAnimationIndex(0);
	}
	if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_UP) == GLFW_PRESS){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, 0.02));
		mayowModelAnimate->setAnimationIndex(0);
	}
	else if (GLFWManager::inputManager->getStatusKey(GLFW_KEY_DOWN) == GLFW_PRESS){
		modelMatrixMayow = glm::translate(modelMatrixMayow, glm::vec3(0.0, 0.0, -0.02));
		mayowModelAnimate->setAnimationIndex(0);
	}

	if(GLFWManager::inputManager->getStatusKey(GLFW_KEY_SPACE) == GLFW_PRESS){
		mayowModelAnimate->startJump();
	}

	mayowModelAnimate->getModelMatrix() = modelMatrixMayow;
	GLFWManager::inputManager->getCamera()->updateCamera();
}

void introController(int modelSelected){
	if(!iniciaPartida){
		bool presionarEnter = GLFWManager::inputManager->getStatusKey(GLFW_KEY_ENTER) == GLFW_PRESS;
		if(textureIntroActive == textureIntro1.get() && presionarEnter){
			iniciaPartida = true;
			textureIntroActive = textureScreen.get();
			boxScreen->setShader(&shaderViewDepth);
		}
		else if(!presionarOpcion && GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			presionarOpcion = true;
			if(textureIntroActive == textureIntro1.get())
				textureIntroActive = textureIntro2.get();
			else if(textureIntroActive == textureIntro2.get())
				textureIntroActive = textureIntro1.get();
		}
		else if( GLFWManager::inputManager->getStatusKey(GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
			presionarOpcion = false;
	}
}