#include <assimp/postprocess.h>

#include "Headers/Model.hpp"
#include "Headers/TimeManager.h"
#include "Headers/assimp_glm_helpers.hpp"

Model::Model(Shader* shader_ptr, const std::string & path): Renderable(shader_ptr) {
	this->loadModel(path);
}

void Model::render(glm::mat4 parentTrans) {
	float runningTime = TimeManager::Instance().GetRunningTime();
	shader_ptr->turnOn();
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->scale);
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), this->position);
	glm::quat oX = glm::angleAxis<float>(glm::radians(orientation.x), glm::vec3(1.0, 0.0, 0.0));
	glm::quat oY = glm::angleAxis<float>(glm::radians(orientation.y), glm::vec3(0.0, 1.0, 0.0));
	glm::quat oZ = glm::angleAxis<float>(glm::radians(orientation.z), glm::vec3(0.0, 0.0, 1.0));
	glm::quat ori = oZ * oY * oX;
	modelMatrix = parentTrans * translate * glm::mat4_cast(ori) * scale;
    this->shader_ptr->setMatrix4("model", 1, GL_FALSE, glm::value_ptr(modelMatrix));
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		this->meshes[i]->render(runningTime, bones);
		glActiveTexture(GL_TEXTURE0);
	}
    shader_ptr->turnOff();
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

	this->m_GlobalInverseTransform = AssimpGLMHelpers::ConvertMatrixToGLMFormat(scene->mRootNode->mTransformation);
	
	this->m_GlobalInverseTransform = glm::inverse(
			this->m_GlobalInverseTransform);

	// Recupera el path del directorio del archivo.
	this->directory = path.substr(0, path.find_last_of('/'));

	this->readHierarchyData(rootNode, scene->mRootNode);
	
	// Se procesa el nodo raiz recursivamente.
	this->processNode(scene->mRootNode, scene);

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