#ifndef MODEL_H_
#define MODEL_H_

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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "AbstractModel.hpp"
#include "Renderable.hpp"
#include "Mesh.hpp"
#include "Colliders.hpp"

class DLL_PUBLIC Model : public AbstractModel, public Renderable {
public:
	Model(Shader* shader_ptr, const std::string & path, TYPE_COLLIDER typeCollider = BOX);
	void render(glm::mat4 parentTrans = glm::mat4(1.0f));
	void readHierarchyData(AssimpNodeData& dest, const aiNode* src);
	void readMissingBones(const aiAnimation* animation);
	void getFinalBoneMatrix(std::string nodeNameToFind, AssimpNodeData& node, glm::mat4& nodeFondTransform, bool& nodeFound, glm::mat4 parentTansform);
	void getBonesMatrix(AssimpNodeData& node, std::vector<glm::mat4>& transforms, glm::mat4 parentTansform);

	void setAnimationIndex(int animationIndex){
		for(int i = 0; i < meshes.size(); i++)
			this->meshes[i]->setAnimationIndex(animationIndex);
	}

	AssimpNodeData& getRootNode(){
		return rootNode;
	}

	Collider* getCollider() { return collider; }

protected:
	void loadModel(const std::string & path);
	Assimp::Importer importer;
	const aiScene* scene;
	std::vector<std::shared_ptr<Mesh>> meshes;
	glm::mat4 m_GlobalInverseTransform;
	int animationIndex = 0;

private:
	std::string directory;
	std::map<std::string, Bone> bones;
	AssimpNodeData rootNode;
	TYPE_COLLIDER typeCollider;
	Collider* collider;
	
	void processNode(aiNode* node, const aiScene* scene);
	void createCollider();
	void updateCollider();
	void updateColliderFromBones(AssimpNodeData& node, glm::vec3& mins, glm::vec3& maxs, glm::mat4 parentTansform);
};

#endif /* MODEL_H_ */
