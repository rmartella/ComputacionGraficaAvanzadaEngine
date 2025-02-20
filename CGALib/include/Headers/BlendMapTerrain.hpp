/*
 * Terrain.h
 *
 *  Created on: Nov 14, 2019
 *      Author: rey
 */

#ifndef BLENDMAPTERRAIN_H_
#define BLENDMAPTERRAIN_H_

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

#include <map>
#include "SimpleTerrain.hpp"
#include "Texture2D.hpp"


class DLL_PUBLIC BlendMapTerrain : public SimpleTerrain {
public:
  enum TEXTURE_BLENDMAP_TERRAIN {
      TEXTURE_BACKGROUND,
      TEXTURE_R,
      TEXTURE_G,
      TEXTURE_B,
      TEXTURE_BLENDMAP
  };
	BlendMapTerrain(Shader* shader_ptr,float yScale, float yShift, std::string heightMap, 
    std::map<TEXTURE_BLENDMAP_TERRAIN, std::string> blendMapTextures);
  void render() override;

private:
  std::shared_ptr<Texture2D> textureR;
  std::shared_ptr<Texture2D> textureG;
  std::shared_ptr<Texture2D> textureB;
  std::shared_ptr<Texture2D> textureBlendMap;
};

#endif /* SRC_BLENDMAPTERRAIN_H_ */
