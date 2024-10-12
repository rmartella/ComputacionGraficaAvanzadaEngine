#ifndef HEADERS_MESH_H_
#define HEADERS_MESH_H_

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

// Std. Includes
#include <vector>

#include <assimp/mesh.h>
#include <assimp/material.h>

#include "Drawable.hpp"
#include "Bone.hpp"
#include "Texture2D.hpp"
#include "assimp_glm_helpers.hpp"
#include "Shader.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

struct AssimpNodeData{
  std::string name;
  glm::mat4 transformation;
  std::vector<AssimpNodeData> children;
};

class DLL_PUBLIC Mesh : public Drawable {
public:

  Mesh(const aiMesh *mesh, const aiMaterial *material, const std::string path,
    const aiScene* scene, const AssimpNodeData& rootNode, Shader *shader);
  ~Mesh();
  void render(float timeInSeconds, std::map<std::string, Bone>& bones);
  bool getFinalBoneMatrix(std::string nodeName, glm::mat4& boneMatrix);

  void setAnimationIndex(int animationIndex){
		this->animationIndex = animationIndex;
	}

protected:

  GLuint VBOBones;
  std::vector<std::pair<aiTextureType, std::shared_ptr<Texture2D>>> textures;

private:
  struct BoneInfo {
    int id;
    glm::mat4 boneOffset;
    BoneInfo() = default;
  };

  std::string path;
  std::map<std::string, BoneInfo> boneInfoMap;
  Shader* shader_ptr;

  const AssimpNodeData& rootNode;
  aiAnimation** animations;
  u_int numAnimations;
  int animationIndex = 0;

  struct VertexBoneData {
		unsigned int IDs[4];
		float Weights[4];
		void AddBoneData(unsigned int boneID, float weight) {
			for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++) {
				if (Weights[i] == 0.0) {
					IDs[i] = boneID;
					Weights[i] = weight;
					return;
				}
			}
		}
	};
  
  void processMesh(const aiMesh* mesh, const aiMaterial* material, std::map<std::string, BoneInfo>& boneInfoMap);
  void loadMaterialTextures(const aiMaterial* mat, const aiTextureType type);
  void extractBoneWeight(const aiMesh* mesh, std::map<std::string, BoneInfo>& boneInfoMap);

  void readHierarchyData(AssimpNodeData& dest, const aiNode* src);
  const aiNodeAnim* findNodeAnim(const aiAnimation *pAnimation, const std::string nodeName);
  void calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform, 
	  const aiAnimation *pAnimation, float animationTime, std::vector<glm::mat4>& finalBoneMatrices, 
	  std::map<std::string, Bone> &bones);

};

#endif /* HEADERS_MESH_H_ */
