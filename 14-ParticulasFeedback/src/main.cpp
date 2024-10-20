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
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture2D.hpp"
#include "Headers/TextureCube.hpp"

#include "Headers/Lighting.hpp"

Shader shader;
//Shader con skybox
Shader shaderSkybox;
// Shader para dibujar un objeto con solo textura
Shader shaderTexture;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;

float distanceFromTarget = 7.0;

std::shared_ptr<Sphere> esfera1;
std::shared_ptr<Sphere> esfera2;

std::shared_ptr<Box> box1;
Box* boxCollider;
std::shared_ptr<Sphere> sphereCollider;

std::shared_ptr<SkyBoxSphere> skyBoxSphere;

std::shared_ptr<Texture2D> textureLanding;

std::shared_ptr<Model> modelRock;
std::shared_ptr<Model> modelMay;
std::shared_ptr<BlendMapTerrain> terrain;

LightManager lightManager;

GLFWManager glfwManager;

double deltaTime;
double currTime, lastTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

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

	esfera1 = std::make_shared<Sphere>(&shaderTexture, 20, 20);
	esfera2 = std::make_shared<Sphere>(&shaderTexture, 20, 20, 0.06);

	box1 = std::make_shared<Box>(&shaderTexture);

	boxCollider = new Box(&shaderTexture);
	boxCollider->enableWireMode();

	sphereCollider = std::make_shared<Sphere>(&shaderTexture, 20, 20);
	sphereCollider->enableWireMode();

	textureLanding = std::make_shared<Texture2D>("../Textures/landingPad.jpg", true);

	skyBoxSphere = std::make_shared<SkyBoxSphere>(&shaderSkybox, std::vector<std::pair<GLenum, std::string>>{
		{GL_TEXTURE_CUBE_MAP_POSITIVE_X, "../Textures/mp_bloodvalley/blood-valley_ft.tga"},
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "../Textures/mp_bloodvalley/blood-valley_bk.tga"},
		{GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "../Textures/mp_bloodvalley/blood-valley_up.tga"},
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "../Textures/mp_bloodvalley/blood-valley_dn.tga"},
		{GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "../Textures/mp_bloodvalley/blood-valley_rt.tga"},
		{GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "../Textures/mp_bloodvalley/blood-valley_lf.tga"}
	}, 20);

	modelRock = std::make_shared<Model>(&shaderMulLighting, "../models/rock/rock.obj", SPHERE);
	modelRock->getCollider()->setRenderableCollider(sphereCollider.get());

	modelMay = std::make_shared<Model>(&shaderMulLighting, "../models/boblampclean/boblampclean.md5mesh");
	modelMay->setAnimationIndex(0);
	modelMay->getCollider()->setRenderableCollider(boxCollider);
	//modelMay->enableWireMode();

	lightManager.addDirectionalLight(
		Light(glm::vec3(0.2), glm::vec3(0.5), glm::vec3(0.2)), 
		glm::vec3(-0.707106781, -0.707106781, 0.0));

	lightManager.addPointLight(
		Light(glm::vec3(0.2, 0.16, 0.01), glm::vec3(0.4, 0.32, 0.02), glm::vec3(0.6, 0.58, 0.03)), 
		glm::vec3(-7.03, 0, -19.14), 1.0, 0.009, 0.002);


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
}

void destroy() {
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderSkybox.destroy();
	delete boxCollider;
}

void prepareScene(){
}

void prepareDepthScene(){
}

void renderSolidScene(){

	textureLanding->bind(GL_TEXTURE0);
	esfera1->setPosition(glm::vec3(-2.0, 2.0, -2.0));
	//esfera1->render();

	box1->setPosition(glm::vec3(0.0, 2.0, -2.0));
	//box1->render();

	modelRock->setPosition(glm::vec3(4.0, 2.0, -2.0));
	modelRock->render();

	modelMay->setPosition(glm::vec3(10.0, 2.0, -2.0));
	modelMay->setScale(glm::vec3(0.021));
	modelMay->render();
	//modelMay->getCollider()->getRenderableCollider()->render();
	//boxCollider->render();

	glm::mat4 boneMatrix;
	bool nodeFound = false;
	modelMay->getFinalBoneMatrix("LHandEX", modelMay->getRootNode(), boneMatrix, nodeFound, glm::mat4(1.0));
	if(nodeFound){
		textureLanding->bind(GL_TEXTURE0);
		esfera2->setPosition(modelMay->getModelMatrix() *  boneMatrix[3]);
		esfera2->render();
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

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = glfwManager.processInput(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) GLFWManager::screenWidth / (float) GLFWManager::screenHeight, 0.01f, 100.0f);
		Camera* camera = GLFWManager::inputManager->getCamera();
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
		//shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		//shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		//shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		//shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));
		
		/*shaderMulLighting.setInt("spotLightCount", 0);
		shaderMulLighting.setInt("pointLightCount", 0);*/

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