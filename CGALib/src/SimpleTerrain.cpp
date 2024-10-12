/*
 * Terrain.cpp
 *
 *  Created on: Nov 14, 2019
 *      Author: rey
 */

#include "Headers/SimpleTerrain.hpp"

SimpleTerrain::SimpleTerrain(Shader* shader_ptr, float yScale, float yShift, std::string heightMap, std::string background) : 
	BaseTerrain(shader_ptr, yScale, yShift, heightMap), scaleUVTerrain(1.0) {
	textureBackground = std::make_shared<Texture2D>(background);
}

SimpleTerrain::SimpleTerrain(Shader* shader_ptr,float yScale, float yShift, std::string heightMap) : BaseTerrain(shader_ptr, yScale, yShift, heightMap) {
}

void SimpleTerrain::render(glm::mat4 parentTrans) {
	textureBackground->bind(GL_TEXTURE0);
	shader_ptr->setVectorFloat2("scaleUV", glm::value_ptr(scaleUVTerrain));
	BaseTerrain::render(parentTrans);
	glBindTexture(GL_TEXTURE_2D, 0);
}

