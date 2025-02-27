#ifndef SPHERE_H
#define SPHERE_H

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

#define _USE_MATH_DEFINES
#include <cmath>
#include "SimpleModel.h"
#include "ObjectCollider.h"
#include "TerrainAnimator.h"

class DLL_PUBLIC Sphere : public SimpleModel, public ObjectCollider, public TerrainAnimator
{
public:
	Sphere(Shader* shader_ptr, int slices, int stacks, 
        float ratio = 0.5, BaseTerrain *terrain = nullptr);
    void render() override;
};

#endif // SPHERE_H
