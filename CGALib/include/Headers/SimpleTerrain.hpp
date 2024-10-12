/*
 * Terrain.h
 *
 *  Created on: Nov 14, 2019
 *      Author: rey
 */

#ifndef SIMPLETERRAIN_H_
#define SIMPLETERRAIN_H_

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

#include "BaseTerrain.hpp"
#include "Texture2D.hpp"

class DLL_PUBLIC SimpleTerrain : public BaseTerrain {
public:
	SimpleTerrain(Shader* shader_ptr,float yScale, float yShift, std::string heightMap, std::string background);
  void render(glm::mat4 parentTrans = glm::mat4(1.0f)) override;
  void setScaleUVTerrain(glm::vec2 scaleUVTerrain) { this->scaleUVTerrain = scaleUVTerrain; }

protected:
  SimpleTerrain(Shader* shader_ptr,float yScale, float yShift, std::string heightMap);
  std::shared_ptr<Texture2D> textureBackground;

private:
  glm::vec2 scaleUVTerrain;
};

#endif /* SRC_SIMPLETERRAIN_H_ */
