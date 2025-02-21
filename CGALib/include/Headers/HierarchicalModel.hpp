#pragma once
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

#include "Renderable.hpp"
#include "TerrainAnimator.hpp"

class DLL_PUBLIC HierarchicalModel : public Renderable, public ObjectCollider, public TerrainAnimator {
public:
	HierarchicalModel(Shader* shader_ptr, BaseTerrain* terrain = nullptr, TYPE_COLLIDER typeCollider = BOX) : Renderable(shader_ptr), ObjectCollider(typeCollider), TerrainAnimator(terrain) {}
    ~HierarchicalModel(){
        initCollider = nullptr;
        collider = nullptr;
    }
    std::vector<float>& getJoints() { return joints; }

protected:
    std::vector<float> joints;
};


