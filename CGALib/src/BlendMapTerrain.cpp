/*
 * Terrain.cpp
 *
 *  Created on: Nov 14, 2019
 *      Author: rey
 */

#include "Headers/BlendMapTerrain.hpp"

BlendMapTerrain::BlendMapTerrain(Shader* shader_ptr, float yScale, float yShift, std::string heightMap, 
	std::map<TEXTURE_BLENDMAP_TERRAIN, std::string> blendMapTextures) : 
	SimpleTerrain(shader_ptr, yScale, yShift, heightMap) {
	auto backgroundIt = blendMapTextures.find(TEXTURE_BACKGROUND);
	auto rIt = blendMapTextures.find(TEXTURE_R);
	auto gIt = blendMapTextures.find(TEXTURE_G);
	auto bIt = blendMapTextures.find(TEXTURE_B);
	auto blendMapIt = blendMapTextures.find(TEXTURE_BLENDMAP);
	
	if(backgroundIt != blendMapTextures.end())
		textureBackground = std::make_shared<Texture2D>(backgroundIt->second);
	if(rIt != blendMapTextures.end())
		textureR= std::make_shared<Texture2D>(rIt->second);
	if(gIt != blendMapTextures.end())
		textureG = std::make_shared<Texture2D>(gIt->second);
	if(bIt != blendMapTextures.end())
		textureB = std::make_shared<Texture2D>(bIt->second);
	if(blendMapIt != blendMapTextures.end())
		textureBlendMap = std::make_shared<Texture2D>(blendMapIt->second);
}

void BlendMapTerrain::render() {
	textureR->bind(GL_TEXTURE1);
	shader_ptr->setInt("rTexture", 1);
	textureG->bind(GL_TEXTURE2);
	shader_ptr->setInt("gTexture", 2);
	textureB->bind(GL_TEXTURE3);
	shader_ptr->setInt("bTexture", 3);
	textureBlendMap->bind(GL_TEXTURE4);
	shader_ptr->setInt("blendMapTexture", 4);
	SimpleTerrain::render();
	glBindTexture(GL_TEXTURE_2D, 0);
}