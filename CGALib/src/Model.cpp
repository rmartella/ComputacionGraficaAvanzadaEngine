#include <assimp/postprocess.h>
#include "Headers/Model.hpp"
#include "Headers/TimeManager.h"
#include "Headers/assimp_glm_helpers.hpp"

Model::Model(Shader* shader_ptr, const std::string & path, BaseTerrain* terrain, TYPE_COLLIDER typeCollider): 
	ModelBase(shader_ptr), ObjectCollider(typeCollider), TerrainAnimator(terrain) {
	this->loadModel(path);
}

void Model::render() {
	GLint polygonMode[2];  // Almacena los modos para GL_FRONT y GL_BACK
	glGetIntegerv(GL_POLYGON_MODE, polygonMode);
	if(wiredMode)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	float runningTime = TimeManager::Instance().GetRunningTime();
	shader_ptr->turnOn();
	TerrainAnimator::animate(modelMatrix);
	glm::mat4 finalModelMatrix = glm::scale(glm::mat4(modelMatrix), scale);
    this->shader_ptr->setMatrix4("model", 1, GL_FALSE, glm::value_ptr(finalModelMatrix));
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		this->meshes[i]->render(runningTime, bones, this->m_GlobalInverseTransform);
		glActiveTexture(GL_TEXTURE0);
	}
	this->updateCollider();
    shader_ptr->turnOff();
	glPolygonMode(GL_FRONT, polygonMode[0]);
    glPolygonMode(GL_BACK, polygonMode[1]);
}

void Model::loadModel(const std::string & path) {
	// Lee el archivo via ASSIMP
	scene = importer.ReadFile(path.c_str(),
			aiProcess_Triangulate | aiProcess_FlipUVs
					| aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	// Revisa errores
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE
			|| !scene->mRootNode) // if is Not Zero
			{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString()
				<< std::endl;
		return;
	}

	/*this->m_GlobalInverseTransform = AssimpGLMHelpers::ConvertMatrixToGLMFormat(scene->mRootNode->mTransformation);
	this->m_GlobalInverseTransform = glm::inverse(
			this->m_GlobalInverseTransform);*/
			
	this->m_GlobalInverseTransform = glm::mat4(1.0f);

	// Recupera el path del directorio del archivo.
	this->directory = path.substr(0, path.find_last_of('/'));

	this->readHierarchyData(rootNode, scene->mRootNode);
	
	// Se procesa el nodo raiz recursivamente.
	this->processNode(scene->mRootNode, scene);

	this->createCollider();

	if(scene->mNumAnimations > 0)
		this->readMissingBones(scene->mAnimations[0]);

}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// Procesa cada maya del nodo actual
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		aiMaterial* material = nullptr;
		if (mesh->mMaterialIndex >= 0)
			material = scene->mMaterials[mesh->mMaterialIndex];

		this->meshes.push_back(std::make_shared<Mesh>(
			mesh, material, this->directory, 
			scene, this->rootNode,shader_ptr
		));
	}
	for (GLuint i = 0; i < node->mNumChildren; i++)
		this->processNode(node->mChildren[i], scene);
}

void Model::getFinalBoneMatrix(std::string nodeNameToFind, AssimpNodeData& node, glm::mat4& nodeFondTransform, bool& nodeFound, glm::mat4 parentTansform){
	std::string nodeName = node.name;
	auto boneIt = bones.find(nodeName);
	glm::mat4 nodeTransform = node.transformation;
	if (boneIt != bones.end()) 
		nodeTransform = boneIt->second.getLocalTransform();
	
	if(node.name.compare(nodeNameToFind) == 0){
		nodeFondTransform = parentTansform * nodeTransform;
		nodeFound = true;
		return;
	}

	for(int i = 0; i < node.children.size(); i++){
		if(nodeFound)
			return;
		this->getFinalBoneMatrix(nodeNameToFind, node.children[i], nodeFondTransform, nodeFound, parentTansform * nodeTransform);
	}

}

void Model::getBonesMatrix(AssimpNodeData& node, std::vector<glm::mat4>& transforms, glm::mat4 parentTansform){
	std::string nodeName = node.name;
	auto boneIt = bones.find(nodeName);

	glm::mat4 nodeTransform = node.transformation;
	if (boneIt != bones.end()) {
		nodeTransform = boneIt->second.getLocalTransform();
	}
	
	transforms.push_back(parentTansform * nodeTransform);

	for(int i = 0; i < node.children.size(); i++){
		this->getBonesMatrix(node.children[i], transforms, parentTansform * nodeTransform);
	}
}

void Model::readHierarchyData(AssimpNodeData& dest, const aiNode* src) {
	assert(src);
	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
	//dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++) {
		AssimpNodeData newData;
		readHierarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}

void Model::readMissingBones(const aiAnimation* animation){
	int size = animation->mNumChannels;

	//auto& boneInfoMap = this->boneInfoMap;//getting m_BoneInfoMap from Model class
	//int boneCount = boneInfoMap.size(); //getting the m_BoneCounter from Model class

	//reading channels(bones engaged in an animation and their keyframes)
	for (int i = 0; i < size; i++){
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		/*if (boneInfoMap.find(boneName) == boneInfoMap.end() && boneName.compare("Armature") == 0){
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}*/
		//if (boneInfoMap.find(boneName) != boneInfoMap.end())
		bones[boneName] = Bone(boneName, 0);
	}

	//this->boneInfoMap = boneInfoMap;
}

void Model::createCollider() {
	glm::vec3 mins = glm::vec3(FLT_MAX);
	glm::vec3 maxs = glm::vec3(-FLT_MAX);
	for(u_int i = 0; i < meshes.size(); i++){
		for(u_int j = 0; j < 3; j++){
			if (meshes[i]->getMins()[j] < mins[j])
				mins[j] = meshes[i]->getMins()[j];
			if (meshes[i]->getMaxs()[j] > maxs[j])
				maxs[j] = meshes[i]->getMaxs()[j];
		}
	}

	switch (typeCollider) {
	case BOX:
		this->initCollider = new OBBCollider();
		this->collider = new OBBCollider();
		break;
	case SPHERE:
		this->initCollider = new SBBCollider();
		this->collider = new SBBCollider();
		break;
	}
	this->initCollider->updateCollider(mins, maxs);	
}

void Model::updateCollider(){
	if(bones.size() > 0){
		glm::vec3 mins = glm::vec3(FLT_MAX);
		glm::vec3 maxs = glm::vec3(-FLT_MAX);
		this->updateColliderFromBones(rootNode, mins, maxs, glm::mat4(1.0f));
		this->initCollider->updateCollider(mins, maxs);
	}
	glm::mat4 finalModelMatrix = modelMatrix * glm::scale(glm::mat4(1.0), scale);
	collider->updateLogicCollider(initCollider, finalModelMatrix);
}

void Model::updateColliderFromBones(AssimpNodeData& node, glm::vec3& mins, glm::vec3& maxs, glm::mat4 parentTansform){
	std::string nodeName = node.name;
	auto boneIt = bones.find(nodeName);
	glm::mat4 nodeTransform = node.transformation;
	if (boneIt != bones.end()) 
		nodeTransform = boneIt->second.getLocalTransform();

	for(uint i = 0; i < 3; i++){
		mins[i] = std::min(mins[i], parentTansform[3][i]);
		maxs[i] = std::max(maxs[i], parentTansform[3][i]);
	}

	for(int i = 0; i < node.children.size(); i++)
		this->updateColliderFromBones(node.children[i], mins, maxs, parentTansform * nodeTransform);
}