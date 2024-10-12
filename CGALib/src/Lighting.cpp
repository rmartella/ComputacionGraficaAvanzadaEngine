#include <glm/gtc/type_ptr.hpp>
#include "Headers/Lighting.hpp"

void LightManager::addDirectionalLight(const glm::vec3& ambient, const glm::vec3& difuse, 
	const glm::vec3& specular, const glm::vec3& direction){
	this->directionalLights = DirectionalLight(ambient, difuse, specular, direction);
}

void LightManager::addDirectionalLight(const Light& light, const glm::vec3& direction){
	this->directionalLights = DirectionalLight(light, direction);
}

void LightManager::addPointLight(const glm::vec3& ambient, const glm::vec3& difuse, const glm::vec3& specular, const glm::vec3& position, 
	float constant, float linear, float quadratic) {
	pointLights.push_back(PointLight(ambient, difuse, specular, position, constant, linear, quadratic));
}

void LightManager::addPointLight(const Light& light, const glm::vec3& position, float constant, float linear, float quadratic) {
	pointLights.push_back(PointLight(light, position, constant, linear, quadratic));
}

void LightManager::addSpotLight(const glm::vec3& ambient, const glm::vec3& difuse, const glm::vec3& specular, const glm::vec3& direction, 
	const glm::vec3& position, float constant, float linear, float quadratic, 
	float cutOff, float outerCutOff){
	spotLights.push_back(SpotLight(ambient, difuse, specular, direction, position, constant, linear, quadratic, cutOff, outerCutOff));
}

void LightManager::applyLighting(std::vector<Shader*> shaders_ptr) {
	for(u_int i = 0; i < shaders_ptr.size(); i++){
		shaders_ptr[i]->setVectorFloat3("directionalLight.light.ambient", 
			glm::value_ptr(directionalLights.getAmbiente()));
		shaders_ptr[i]->setVectorFloat3("directionalLight.light.diffuse", 
			glm::value_ptr(directionalLights.getDifuse()));
		shaders_ptr[i]->setVectorFloat3("directionalLight.light.specular", 
			glm::value_ptr(directionalLights.getSpecular()));
		shaders_ptr[i]->setVectorFloat3("directionalLight.direction", 
			glm::value_ptr(directionalLights.getDirection()));

		shaders_ptr[i]->setInt("pointLightCount", pointLights.size());
		for(u_int j = 0; j < pointLights.size(); j++){
			shaders_ptr[i]->setVectorFloat3("pointLights[" + std::to_string(j) + "].light.ambient", glm::value_ptr(pointLights[j].getAmbiente()));
			shaders_ptr[i]->setVectorFloat3("pointLights[" + std::to_string(j) + "].light.diffuse", glm::value_ptr(pointLights[j].getDifuse()));
			shaders_ptr[i]->setVectorFloat3("pointLights[" + std::to_string(j) + "].light.specular", glm::value_ptr(pointLights[j].getSpecular()));
			shaders_ptr[i]->setVectorFloat3("pointLights[" + std::to_string(j) + "].position", glm::value_ptr(pointLights[j].getPosition()));
			shaders_ptr[i]->setFloat("pointLights[" + std::to_string(j) + "].constant", pointLights[j].getConstant());
			shaders_ptr[i]->setFloat("pointLights[" + std::to_string(j) + "].linear", pointLights[j].getLinear());
			shaders_ptr[i]->setFloat("pointLights[" + std::to_string(j) + "].quadratic", pointLights[j].getQuadratic());
		}

		shaders_ptr[i]->setInt("spotLightCount", spotLights.size());
		for(u_int j = 0; j < spotLights.size(); j++){
			shaders_ptr[i]->setVectorFloat3("spotLights[" + std::to_string(j) + "].light.ambient", glm::value_ptr(spotLights[j].getAmbiente()));
			shaders_ptr[i]->setVectorFloat3("spotLights[" + std::to_string(j) + "].light.diffuse", glm::value_ptr(spotLights[j].getDifuse()));
			shaders_ptr[i]->setVectorFloat3("spotLights[" + std::to_string(j) + "].light.specular", glm::value_ptr(spotLights[j].getSpecular()));
			shaders_ptr[i]->setVectorFloat3("spotLights[" + std::to_string(j) + "].position", glm::value_ptr(spotLights[j].getPosition()));
			shaders_ptr[i]->setVectorFloat3("spotLights[" + std::to_string(j) + "].direction", glm::value_ptr(spotLights[j].getDirection()));
			shaders_ptr[i]->setFloat("spotLights[" + std::to_string(j) + "].constant", spotLights[j].getConstant());
			shaders_ptr[i]->setFloat("spotLights[" + std::to_string(j) + "].linear", spotLights[j].getLinear());
			shaders_ptr[i]->setFloat("spotLights[" + std::to_string(j) + "].quadratic", spotLights[j].getQuadratic());
			shaders_ptr[i]->setFloat("spotLights[" + std::to_string(j) + "].cutOff", cos(glm::radians(spotLights[j].getCutOff())));
			shaders_ptr[i]->setFloat("spotLights[" + std::to_string(j) + "].outerCutOff", cos(glm::radians(spotLights[j].getOuterCutOff())));
		}
	}
}