/*
 * Terrain.h
 *
 *  Created on: Nov 14, 2019
 *      Author: rey
 */
#pragma once

#ifndef BASETERRAIN_H_
#define BASETERRAIN_H_

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif

#include "SimpleModel.hpp"
#include "Texture2D.hpp"

class DLL_PUBLIC BaseTerrain : public SimpleModel {
public:
	BaseTerrain(Shader* shader_ptr,float yScale, float yShift, std::string heightMap);
	virtual ~BaseTerrain();
	float getHeightTerrain(float worldX, float worldZ);
	glm::vec3 getNormalTerrain(float worldX, float worldZ);
protected:
  	float yScale, yShift;

private:
	float ** heights;
	glm::vec3 ** normals;
 	Texture2D* textureHeightMap;
  	void init();
	glm::vec3 computeNormal(int x, int z, unsigned char * data, int imageWidth, int imageHeight, int numeroCanales);
	float getHeight(int x, int z, unsigned char * data, int imageWidth, int imageHeight, int numeroCanales);
	float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
	glm::vec3 barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
			glm::vec2 pos, glm::vec3 n1, glm::vec3 n2, glm::vec3 n3);
};

#endif /* SRC_BASETERRAIN_H_ */
