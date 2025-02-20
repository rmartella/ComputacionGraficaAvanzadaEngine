#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

// program include
#include "Headers/TimeManager.h"

#include "Headers/GLFWManager.hpp"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.hpp"
#include "Headers/Cylinder.hpp"
#include "Headers/Box.hpp"
#include "Headers/Model.hpp"
#include "Headers/SkyBoxSphere.hpp"
#include "Headers/BlendMapTerrain.hpp"

#include "Headers/FirstPersonCamera.hpp"
#include "Headers/ThirdPersonCamera.hpp"

#include "Headers/hmodels/Eclipse.hpp"
#include "Headers/hmodels/DarthVader.hpp"
#include "Headers/hmodels/Lambo.hpp"
#include "Headers/hmodels/Heli.hpp"

#include "Headers/KeyFrameAnimator.hpp"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture2D.hpp"
#include "Headers/TextureCube.hpp"

#include "Headers/Lighting.hpp"

#include "Headers/sm/eclipse.h"
#include "Headers/sm/lambo.h"
#include "Headers/sm/heli.h"

bool availableSave = true;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
// Shader para dibujar un objeto con solo textura
Shader shaderTexture;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;

std::shared_ptr<Sphere> esfera1;
std::shared_ptr<Sphere> esfera2;

std::shared_ptr<Box> box1;
std::shared_ptr<Sphere> sphereCollider;
std::shared_ptr<Box> boxCollider;

std::shared_ptr<SkyBoxSphere> skyBoxSphere;

std::shared_ptr<Texture2D> textureLanding;

std::shared_ptr<Model> modelRock;
std::shared_ptr<Model> modelAircraft;

std::shared_ptr<Model> mayowModelAnimate;
std::shared_ptr<Model> cyborgModelAnimate;
std::shared_ptr<Model> cowboyModelAnimate;
std::shared_ptr<Model> guardianModelAnimate;

std::shared_ptr<Eclipse> eclipse;
std::shared_ptr<Lambo> lambo;
std::shared_ptr<Heli> heli;
std::shared_ptr<DarthVader> darthVader;

std::shared_ptr<KeyFrameAnimator> darthAnimation;
std::shared_ptr<KeyFrameAnimator> darthAnimation2;

std::shared_ptr<BlendMapTerrain> terrain;

LightManager lightManager;

GLFWManager glfwManager;

double deltaTime;
double currTime, lastTime;

// Jump variables
bool isJump = false;
float GRAVITY = 1.81;
double tmv = 0;
double startTimeJump = 0;

// Se definen todos las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();

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

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	glfwManager.init(width, height, strTitle, bFullScreen);
	
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");

	std::map<BlendMapTerrain::TEXTURE_BLENDMAP_TERRAIN, std::string> blendMapTextures = {
		{BlendMapTerrain::TEXTURE_BACKGROUND, "../Textures/grassy2.png"},
		{BlendMapTerrain::TEXTURE_R, "../Textures/mud.png"},
		{BlendMapTerrain::TEXTURE_G, "../Textures/grassFlowers.png"},
		{BlendMapTerrain::TEXTURE_B, "../Textures/path.png"},
		{BlendMapTerrain::TEXTURE_BLENDMAP, "../Textures/blendMap.png"}
	};
	terrain = std::make_shared<BlendMapTerrain>(&shaderTerrain, 32.0f, -16.0f, "../Textures/heightmap.png", blendMapTextures);
	terrain->setScaleUVTerrain(glm::vec2(35.0f));
	terrain->setScale(glm::vec3(0.78125));

	boxCollider = std::make_shared<Box>(&shaderTexture);
	boxCollider->enableWireMode();

	sphereCollider = std::make_shared<Sphere>(&shaderTexture, 20, 20);
	sphereCollider->enableWireMode();
	
	esfera1 = std::make_shared<Sphere>(&shaderTexture, 5, 5, 0.5, terrain.get());
	esfera1->getCollider()->setRenderableCollider(sphereCollider.get());
	
	esfera2 = std::make_shared<Sphere>(&shaderTexture, 20, 20, 0.06);

	box1 = std::make_shared<Box>(&shaderTexture);

	textureLanding = std::make_shared<Texture2D>("../Textures/landingPad.jpg", true);

	skyBoxSphere = std::make_shared<SkyBoxSphere>(&shaderSkybox, std::vector<std::pair<GLenum, std::string>>{
		{GL_TEXTURE_CUBE_MAP_POSITIVE_X, "../Textures/mp_bloodvalley/blood-valley_ft.tga"},
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "../Textures/mp_bloodvalley/blood-valley_bk.tga"},
		{GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "../Textures/mp_bloodvalley/blood-valley_up.tga"},
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "../Textures/mp_bloodvalley/blood-valley_dn.tga"},
		{GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "../Textures/mp_bloodvalley/blood-valley_rt.tga"},
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "../Textures/mp_bloodvalley/blood-valley_lf.tga"}
	}, 20);

	modelRock = std::make_shared<Model>(&shaderMulLighting, "../models/rock/rock.obj", terrain.get(), SPHERE);
	modelRock->setPosition(glm::vec3(-3.0, 0.0, 2.0));
	modelRock->getCollider()->setRenderableCollider(sphereCollider.get());

	modelAircraft = std::make_shared<Model>(&shaderMulLighting, "../models/Aircraft_obj/E 45 Aircraft_obj.obj", terrain.get(), BOX);
	modelAircraft->setPosition(glm::vec3(10.0, 2.0, -17.5));
	modelAircraft->setOffsetHeight(3.0f);
	modelAircraft->getCollider()->setRenderableCollider(boxCollider.get());

	eclipse = std::make_shared<Eclipse>(&shaderMulLighting, terrain.get());
	eclipse->setScale(glm::vec3(0.5));
	eclipse->setOrientation(glm::vec3(0, 180.0f, 0.0));
	eclipse->setPosition(glm::vec3(27.5, 0, 30.0));
	eclipse->getCollider()->setRenderableCollider(boxCollider.get());

	lambo = std::make_shared<Lambo>(&shaderMulLighting, terrain.get());
	lambo->setPosition(glm::vec3(23.0, 0.0, 0.0));
	lambo->getCollider()->setRenderableCollider(boxCollider.get());

	heli = std::make_shared<Heli>(&shaderMulLighting);
	heli->setPosition(glm::vec3(5.0, 10.0, -5.0));

	darthVader = std::make_shared<DarthVader>(&shaderMulLighting, terrain.get());
	darthVader->getCollider()->setRenderableCollider(boxCollider.get());
	darthAnimation = std::make_shared<KeyFrameAnimator>("../animations/darth_1.txt", darthVader.get());
	darthAnimation2 = std::make_shared<KeyFrameAnimator>("../animations/darth_2.txt", darthVader.get());

	mayowModelAnimate = std::make_shared<Model>(&shaderMulLighting, "../models/mayow/personaje2.fbx", terrain.get());
	mayowModelAnimate->setPosition(glm::vec3(13.0f, 0.05f, -5.0f));
	mayowModelAnimate->setScale(glm::vec3(0.021));
	mayowModelAnimate->setOrientation(glm::vec3(0, -90.0, 0));
	mayowModelAnimate->getCollider()->setRenderableCollider(boxCollider.get());

	cyborgModelAnimate = std::make_shared<Model>(&shaderMulLighting, "../models/cyborg/cyborg.fbx", terrain.get());
	cyborgModelAnimate->setPosition(glm::vec3(5.0f, 0.05, 0.0f));
	cyborgModelAnimate->setScale(glm::vec3(0.009f));
	cyborgModelAnimate->getCollider()->setRenderableCollider(boxCollider.get());
	
	cowboyModelAnimate = std::make_shared<Model>(&shaderMulLighting, "../models/cowboy/Character Running.fbx", terrain.get());
	cowboyModelAnimate->setPosition(glm::vec3(13.0, 0.05, 0.0));
	cowboyModelAnimate->setScale(glm::vec3(0.0021f));
	cowboyModelAnimate->getCollider()->setRenderableCollider(boxCollider.get());

	guardianModelAnimate = std::make_shared<Model>(&shaderMulLighting, "../models/boblampclean/boblampclean.md5mesh", terrain.get());
	guardianModelAnimate->setPosition(glm::vec3(15, 0.05, 0.0));
	guardianModelAnimate->setScale(glm::vec3(0.04f));
	guardianModelAnimate->getCollider()->setRenderableCollider(boxCollider.get());

	std::shared_ptr<ThirdPersonCamera> camera = std::make_shared<ThirdPersonCamera>();
	camera->setModelTarget(mayowModelAnimate.get());
	camera->setDistanceFromTarget(7.0);
	camera->setSensitivity(1.0);
	GLFWManager::inputManager->setCamera(camera);


	box1->getCollider()->setRenderableCollider(boxCollider.get());

	lightManager.addDirectionalLight(
		Light(glm::vec3(0.2), glm::vec3(0.5), glm::vec3(0.2)), 
		glm::vec3(-0.707106781, -0.707106781, 0.0));

	lightManager.addPointLight(
		Light(glm::vec3(0.2, 0.16, 0.01), glm::vec3(0.4, 0.32, 0.02), glm::vec3(0.6, 0.58, 0.03)), 
		glm::vec3(-7.03, 0, -19.14), 1.0, 0.009, 0.002);
}

void destroy() {
}

void prepareScene(){
}

void prepareDepthScene(){
}

void renderSolidScene(){

	textureLanding->bind(GL_TEXTURE0);
	esfera1->setPosition(glm::vec3(-2.0, 2.0, -2.0));
	esfera1->render();

	box1->setPosition(glm::vec3(0.0, 2.0, -2.0));
	//box1->render();

	modelRock->render();

	modelAircraft->render();

	eclipse->render();
	sm_eclipse(eclipse);

	lambo->render();
	sm_lambo(lambo);

	heli->render();
	sm_heli(heli);

	darthAnimation->animate();
	darthVader->setScale(glm::vec3(0.5));
	darthVader->render();

	darthAnimation2->animate();
	darthVader->render();

	mayowModelAnimate->render();

	cyborgModelAnimate->render();
	
	cowboyModelAnimate->render();

	guardianModelAnimate->render();

	glm::mat4 boneMatrix;
	bool nodeFound = false;
	mayowModelAnimate->getFinalBoneMatrix("LHandEX", mayowModelAnimate->getRootNode(), boneMatrix, nodeFound, glm::mat4(1.0));
	if(nodeFound){
		textureLanding->bind(GL_TEXTURE0);
		esfera2->setPosition(mayowModelAnimate->getModelMatrix() *  boneMatrix[3]);
		//esfera2->render();
	}

	terrain->render();

	//esfera2->setPosition(glm::vec3(-8.0, 2.0, -2.0));
	//esfera2->render();

	/*std::vector<glm::mat4> transforms;
	modelMay->getBonesMatrix(modelMay->getRootNode(), transforms, glm::mat4(1.0));
	glm::mat4 modelMatrixDumy = glm::mat4(1.0);
	modelMatrixDumy = glm::translate(modelMatrixDumy, glm::vec3(-8.0, 2.0, -2.0));
	for(int i = 0; i < transforms.size(); i++){
		//esfera2->render(modelMay->getModelMatrix());
		esfera2->setPosition(modelMay->getModelMatrix() * transforms[i][3]);
		esfera2->render();
	}*/

	skyBoxSphere->render();
}

void renderAlphaScene(bool render = true){
}

void applicationLoop() {
	bool psi = true;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, -10.0);

	std::vector<std::function<void(int)>> callbacks = {
        /*darth_controller,*/ mayow_controller
    };

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = glfwManager.processInput(true, callbacks);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) GLFWManager::screenWidth / (float) GLFWManager::screenHeight, 0.01f, 100.0f);
		Camera* camera = GLFWManager::inputManager->getCamera().get();
		glm::mat4 view = camera->getViewMatrix();

		// Settea la matriz de vista y projection al shader con solo color
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));

		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));

		lightManager.applyLighting({&shaderMulLighting, &shaderTerrain});

		renderSolidScene();

		glfwManager.swapTheBuffers();
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}