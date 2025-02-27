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

#include <string>

#include <assimp/scene.h>

#include <glm/glm.hpp>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

class DLL_PUBLIC Bone {

public:
	
	Bone() = default;
	Bone(const std::string& name, int id);
	void update(const aiNodeAnim *pNodeAnim, float animationTime);

	glm::mat4 getLocalTransform() { return m_LocalTransform; }
	std::string getBoneName() const { return m_Name; }

private:
	glm::mat4 m_LocalTransform;
	std::string m_Name;

	unsigned int findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

	void calcInterpolatedPosition(aiVector3D& Out, float AnimationTime,
			const aiNodeAnim* pNodeAnim);
	void calcInterpolatedRotation(aiQuaternion& Out, float AnimationTime,
			const aiNodeAnim* pNodeAnim);
	void calcInterpolatedScaling(aiVector3D& Out, float AnimationTime,
			const aiNodeAnim* pNodeAnim);

};