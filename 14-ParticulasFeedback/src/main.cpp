#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

// program include
#include "Headers/TimeManager.h"

#include "Headers/GLFWManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/Quad.h"
#include "Headers/Model.h"
#include "Headers/SkyBoxSphere.h"
#include "Headers/BlendMapTerrain.h"

#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

#include "Headers/hmodels/Eclipse.h"
#include "Headers/hmodels/DarthVader.h"
#include "Headers/hmodels/Lambo.h"
#include "Headers/hmodels/Heli.h"

#include "Headers/KeyFrameAnimator.h"

#include "Headers/FontTypeRendering.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture2D.h"
#include "Headers/TextureCube.h"

#include "Headers/Lighting.h"

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
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
// Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;

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

std::shared_ptr<Model> modelLamp1;
std::shared_ptr<Model> modelLamp2;
std::shared_ptr<Model> modelLampPost2;

// Lamps position
std::vector<std::pair<glm::vec3, float>> lamp1 = {
	{ glm::vec3(-7.03, 0, -19.14), -17.0 },
	{ glm::vec3(24.41, 0, -34.57), -82.67 },
	{ glm::vec3(-10.15, 0, -54.1), 23.70 }
};

std::vector<std::pair<glm::vec3, float>> lamp2 = {
	{ glm::vec3(-36.52, 0, -23.24), 21.37 + 90 },
	{ glm::vec3(-52.73, 0, -3.90), -65.0 + 90 }
};

std::shared_ptr<SpotLight> spotLight;

std::shared_ptr<Quad> boxScreen;
std::shared_ptr<FontTypeRendering::FontTypeRendering> modelText;
bool iniciaPartida = false, presionarOpcion = false;
std::map<std::string, glm::vec3> blendingUnsorted = {
	{"aircraft", glm::vec3(0.0)},
	{"lambo", glm::vec3(0.0)},
	{"heli", glm::vec3(0.0)}
};
std::shared_ptr<Texture2D> textureIntro1;
std::shared_ptr<Texture2D> textureIntro2;
std::shared_ptr<Texture2D> textureScreen;
Texture2D* textureIntroActive;

GLFWManager glfwManager;

double deltaTime;
double currTime, lastTime;

// Framesbuffers
GLuint depthMap, depthMapFBO;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
float near_plane = 0.1f, far_plane = 15.0f;

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
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs");

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
	esfera1->setPosition(glm::vec3(-2.0, 2.0, -2.0));
	esfera1->getCollider()->setRenderableCollider(sphereCollider.get());
	
	esfera2 = std::make_shared<Sphere>(&shaderTexture, 20, 20, 0.06);

	box1 = std::make_shared<Box>(&shaderTexture, terrain.get());
	box1->setPosition(glm::vec3(0.0, 2.0, -2.0));
	//box1->getCollider()->setRenderableCollider(boxCollider.get());

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
	darthVader->setPosition(glm::vec3(5.0, 10.0, -5.0));
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

	modelLamp1 = std::make_shared<Model>(&shaderMulLighting, "../models/Street-Lamp-Black/objLamp.obj", terrain.get());
	modelLamp1->setScale(glm::vec3(0.5));
	modelLamp1->getCollider()->setRenderableCollider(boxCollider.get());
	modelLamp2 = std::make_shared<Model>(&shaderMulLighting, "../models/Street_Light/Lamp.obj", terrain.get());
	modelLampPost2 = std::make_shared<Model>(&shaderMulLighting, "../models/Street_Light/LampPost.obj", terrain.get());
	modelLampPost2->getCollider()->setRenderableCollider(boxCollider.get());

	std::shared_ptr<ThirdPersonCamera> camera = std::make_shared<ThirdPersonCamera>();
	camera->setModelTarget(mayowModelAnimate.get());
	camera->setDistanceFromTarget(7.0);
	camera->setSensitivity(1.0);
	GLFWManager::inputManager->setCamera(camera);

	lightManager.addDirectionalLight(
		Light(glm::vec3(0.05, 0.05, 0.05), glm::vec3(0.3, 0.3, 0.3), glm::vec3(0.4, 0.4, 0.4)), 
		glm::vec3(-0.707106781, -0.707106781, 0.0));

	for(int i = 0; i < lamp1.size(); i++){
		glm::vec3 lampPosition = lamp1[i].first;
		lampPosition.y = terrain->getHeightTerrain(lampPosition.x, lampPosition.z) + 10.35 * 0.5;
		lightManager.addPointLight(
			Light(glm::vec3(0.2, 0.16, 0.01), glm::vec3(0.4, 0.32, 0.02), glm::vec3(0.6, 0.58, 0.03)), 
			lampPosition, 1.0, 0.09, 0.02);
	}

	for(int i = 0; i < lamp2.size(); i++){
		float lampOri = lamp2[i].second;
		glm::vec3 lampPosition = lamp2[i].first;
		glm::mat4 matrixAdjustLamp = glm::mat4(1.0);
		matrixAdjustLamp = glm::translate(matrixAdjustLamp, lampPosition);
		matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lampOri), glm::vec3(0, 1, 0));
		matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0));
		matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.75, 0.0, 0));
		lampPosition.y = terrain->getHeightTerrain(lampPosition.x, lampPosition.z) + 5.0 * 1.0;
		lampPosition = matrixAdjustLamp[3];
		lightManager.addPointLight(
			Light(glm::vec3(0.2, 0.16, 0.01), glm::vec3(0.4, 0.32, 0.02), glm::vec3(0.6, 0.58, 0.03)), 
			lampPosition, 1.0, 0.09, 0.02);
	}

	spotLight = std::make_shared<SpotLight>(Light(glm::vec3(0.2, 0.16, 0.01), glm::vec3(0.4, 0.32, 0.02), glm::vec3(0.6, 0.58, 0.03)),
		glm::vec3(0, -1, 0), heli->getModelMatrix()[3], 1.0, 0.009, 0.002, cos(glm::radians(12.5f)), cos(glm::radians(15.0f)));
	lightManager.addSpotLight(spotLight);

	boxScreen = std::make_shared<Quad>(&shaderTexture);
	boxScreen->setScale(glm::vec3(2.0, 2.0, 1.0));
	textureIntro1 = std::make_shared<Texture2D>("../Textures/Intro1.png", true);
	textureIntro2 = std::make_shared<Texture2D>("../Textures/Intro2.png", true);
	textureScreen = std::make_shared<Texture2D>("../Textures/Screen.png", true);
	textureIntroActive = textureIntro1.get();
	modelText = std::make_shared<FontTypeRendering::FontTypeRendering>(WindowManager::screenWidth, WindowManager::screenHeight);
	modelText->Initialize();

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void destroy() {
}

void prepareScene(){
	terrain->setShader(&shaderTerrain);
	
	modelRock->setShader(&shaderMulLighting);

	modelAircraft->setShader(&shaderMulLighting);

	// Eclipse
	eclipse->setShader(&shaderMulLighting);

	// Helicopter
	heli->setShader(&shaderMulLighting);

	// Lamborginhi
	lambo->setShader(&shaderMulLighting);

	// Dart Lego
	darthVader->setShader(&shaderMulLighting);

	//Lamp models
	modelLamp1->setShader(&shaderMulLighting);
	modelLamp2->setShader(&shaderMulLighting);
	modelLampPost2->setShader(&shaderMulLighting);

	//Buzz
	//buzz->setShader(&shaderMulLighting);

	//Grass
	//modelGrass->setShader(&shaderMulLighting);

	//Mayow
	mayowModelAnimate->setShader(&shaderMulLighting);

	//Cowboy
	cowboyModelAnimate->setShader(&shaderMulLighting);

	//Guardian
	guardianModelAnimate->setShader(&shaderMulLighting);

	//Cyborg
	cyborgModelAnimate->setShader(&shaderMulLighting);

}

void prepareDepthScene(){
	terrain->setShader(&shaderDepth);
	
	modelRock->setShader(&shaderDepth);

	modelAircraft->setShader(&shaderDepth);

	// Eclipse
	eclipse->setShader(&shaderDepth);

	// Helicopter
	heli->setShader(&shaderDepth);

	// Lamborginhi
	lambo->setShader(&shaderDepth);

	// Dart Lego
	darthVader->setShader(&shaderDepth);

	//Lamp models
	modelLamp1->setShader(&shaderDepth);
	modelLamp2->setShader(&shaderDepth);
	modelLampPost2->setShader(&shaderDepth);

	//Buzz
	//buzz->setShader(&shaderDepth);

	//Grass
	//modelGrass->setShader(&shaderDepth);

	//Mayow
	mayowModelAnimate->setShader(&shaderDepth);

	//Cowboy
	cowboyModelAnimate->setShader(&shaderDepth);

	//Guardian
	guardianModelAnimate->setShader(&shaderDepth);

	//Cyborg
	cyborgModelAnimate->setShader(&shaderDepth);
}

void renderSolidScene(){

	terrain->render();

	textureLanding->bind(GL_TEXTURE0);
	esfera1->render();

	box1->render();

	modelRock->render();


	eclipse->render();
	sm_eclipse(eclipse);

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
		esfera2->setPosition(glm::scale(mayowModelAnimate->getModelMatrix(), mayowModelAnimate->getScale()) * boneMatrix[3]);
		esfera2->render();
	}

	for(int i = 0; i < lamp1.size(); i++){
		modelLamp1->setPosition(lamp1[i].first);
		modelLamp1->setOrientation(glm::vec3(0, lamp1[i].second, 0));
		modelLamp1->render();
	}

	for(int i = 0; i < lamp2.size(); i++){
		modelLamp2->setPosition(lamp2[i].first);
		modelLamp2->setOrientation(glm::vec3(0, lamp2[i].second, 0));
		modelLamp2->render();
		modelLampPost2->setPosition(lamp2[i].first);
		modelLampPost2->setOrientation(glm::vec3(0, lamp2[i].second, 0));
		modelLampPost2->render();
	}

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
	if(render)
		modelText->render("Texto en OpenGL", -1, 0, 1, 0, 0, 24);

	/**********
	 * Update the position with alpha objects
	 */
	// Update the aircraft
	blendingUnsorted.find("aircraft")->second = modelAircraft->getModelMatrix()[3];
	// Update the lambo
	blendingUnsorted.find("lambo")->second = lambo->getModelMatrix()[3];
	// Update the helicopter
	blendingUnsorted.find("heli")->second = heli->getModelMatrix()[3];

	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for(itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++){
		float distanceFromView = glm::length(GLFWManager::inputManager->getCamera()->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for(std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++){
		if(it->second.first.compare("aircraft") == 0){
			modelAircraft->render();
		}
		else if(it->second.first.compare("lambo") == 0){
			lambo->render();
			sm_lambo(lambo);
		}
		else if(it->second.first.compare("heli") == 0){
			heli->render();
			sm_heli(heli);
			spotLight->getPosition() = heli->getModelMatrix() * glm::vec4(0.0, 0.2, 1.75, 1.0);
		}
	}
	if(render){
		/************Render de imagen de frente**************/
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		textureIntroActive->bind(GL_TEXTURE0);
		boxScreen->render();
	}
	glDisable(GL_BLEND);	
}

void applicationLoop() {
	bool psi = true;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, -10.0);

	std::vector<std::function<void(int)>> callbacks = {
        darth_controller, mayow_controller, introController
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

		if(!iniciaPartida){
			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
			textureIntroActive->bind(GL_TEXTURE0);
			boxScreen->render();
			glfwManager.swapTheBuffers();
			continue;
		}

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) GLFWManager::screenWidth / (float) GLFWManager::screenHeight, 0.01f, 100.0f);
		Camera* camera = GLFWManager::inputManager->getCamera().get();
		glm::mat4 view = camera->getViewMatrix();

		// Projection light shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;
		lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);// Se comenta despúes
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0)); // Se comenta después
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		//view = lightView;
		//projection = lightProjection;

		// Settea la matriz de vista y projection al shader con solo color
		shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(lightView))));

		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));

		lightManager.applyLighting({&shaderMulLighting, &shaderTerrain});

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		prepareDepthScene();
		renderSolidScene();
		renderAlphaScene(false);
		//glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		// reset viewport
		/*glViewport(0, 0, WindowManager::screenWidth, WindowManager::screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render Depth map to quad for visual debugging
		shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setFloat("near_plane", near_plane);
		shaderViewDepth.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		boxScreen->render();*/

		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, WindowManager::screenWidth, WindowManager::screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		renderSolidScene();
		/**********Render de transparencias***************/
		renderAlphaScene();

		glfwManager.swapTheBuffers();
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}