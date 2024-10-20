#include <string>
#include <map>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Headers/Mesh.hpp"

Mesh::Mesh(const aiMesh* mesh, const aiMaterial* material, const std::string path, 
	const aiScene* scene,  const AssimpNodeData& rootNode, Shader * shader) 
	: shader_ptr(shader), path(path), rootNode(rootNode){
	this->processMesh(mesh, material, boneInfoMap);
	this->animations = scene->mAnimations;
	this->numAnimations = scene->mNumAnimations;
}

Mesh::~Mesh(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBOBones);
}

void Mesh::render(float timeInSeconds, std::map<std::string, Bone>& bones, glm::mat4 m_GlobalInverseTransform) {
	// Enlace de las texturas apropiadas
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;
	GLuint heightNr = 1;
	for (GLuint i = 0; i < this->textures.size(); i++) {
		// Recupera el numero de la textura (La N en diffuse_textureN)
		std::string nameUniform("material.");
		aiTextureType type = this->textures[i].first;
		switch (type){
		case aiTextureType_DIFFUSE:
			nameUniform.append("texture_diffuse" + std::to_string(diffuseNr++));
			break;
		case aiTextureType_SPECULAR:
			nameUniform.append("texture_specular" + std::to_string(specularNr++));
			break;
		case aiTextureType_NORMALS:
			nameUniform.append("texture_normal" + std::to_string(normalNr++));
			break;
		case aiTextureType_HEIGHT:
			nameUniform.append("texture_height" + std::to_string(heightNr++));
			break;
		default:
			break;
		}
		// El numero de la textura se enlaza con su nombre y su unidad de textura.
		// Por ejemplo texture_diffuse1, texture_specular1
		glUniform1i(shader_ptr->getUniformLocation(nameUniform), i);

		// Enlace de la textura
		this->textures[i].second->bind(GL_TEXTURE0 + i);
	}

	shader_ptr->setInt("numBones", bones.size());
	if(bones.size() > 0 & numAnimations > 0){
		aiAnimation* animation = animations[animationIndex];
		std::vector<glm::mat4> finalBoneMatrices;
		finalBoneMatrices.resize(bones.size());

		float ticksPerSecond = (float) (
			animations[animationIndex]->mTicksPerSecond != 0 ?
					animations[animationIndex]->mTicksPerSecond : 25.0f);
		float timeInTicks = timeInSeconds * ticksPerSecond;
		float animationTime = fmod(timeInTicks,
				(float) animations[animationIndex]->mDuration);

		this->calculateBoneTransform(&rootNode, glm::mat4(1.0), animation, animationTime,
			finalBoneMatrices, bones);
		for (int i = 0; i < finalBoneMatrices.size(); ++i){
			shader_ptr->setMatrix4("bones[" + std::to_string(i) + "]", 1, GL_FALSE,
				glm::value_ptr(m_GlobalInverseTransform * finalBoneMatrices[i]));
		}
	}

	// Se Dibuja la maya
	Drawable::draw();

	shader_ptr->setInt("numBones", 0);

	// Siempre es buena practica colocar los datos defaults antes de ser configurados.
	for (GLuint i = 0; i < this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::processMesh(const aiMesh* mesh, const aiMaterial* material, std::map<std::string, BoneInfo>& boneInfoMap) {
	std::vector<GLuint> index;
	std::vector<Vertex> vertexArray;
	// Recorre los vertices de cada maya
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		// Compute the AABB
		if (mesh->mVertices[i].x < mins.x)
			mins.x = mesh->mVertices[i].x;
		if (mesh->mVertices[i].x > maxs.x)
			maxs.x = mesh->mVertices[i].x;
		if (mesh->mVertices[i].y < mins.y)
			mins.y = mesh->mVertices[i].y;
		if (mesh->mVertices[i].y > maxs.y)
			maxs.y = mesh->mVertices[i].y;
		if (mesh->mVertices[i].z < mins.z)
			mins.z = mesh->mVertices[i].z;
		if (mesh->mVertices[i].z > maxs.z)
			maxs.z = mesh->mVertices[i].z;

		glm::vec2 text = glm::vec2(0.0f, 0.0f);
		// Texture Coordinates
		if (mesh->mTextureCoords[0]) {// Esto se ejecuta si la maya contiene texturas.
			// Un vertice puede contener hasta 8 diferentes coordenadas de textura, unicamente se considera 
			// que los modelos tiene una coordenada de textura por vertice, y corresponde a la primera en el arreglo.
			text.x = mesh->mTextureCoords[0][i].x;
			text.y = mesh->mTextureCoords[0][i].y;
		}

		vertexArray.push_back(Vertex(
			glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z), 
			text, 
			glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z))
		);
	}
	// Se recorre cada cara de la maya (una cara es un triangulo en la maya) y recupera su correspondiente indice del vertice.
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		// Recupera todos los indices de la cara y los almacena en el vector de indices
		for (GLuint j = 0; j < face.mNumIndices; j++)
			index.push_back(face.mIndices[j]);
	}
	// Process materials
	if (material) {
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN

		this->loadMaterialTextures(material, aiTextureType_DIFFUSE);
		this->loadMaterialTextures(material, aiTextureType_SPECULAR);
		this->loadMaterialTextures(material, aiTextureType_NORMALS);
		this->loadMaterialTextures(material, aiTextureType_HEIGHT);
	}
	this->init(vertexArray, index);
	this->extractBoneWeight(mesh, boneInfoMap);
}

void Mesh::loadMaterialTextures(const aiMaterial* mat, const aiTextureType type) {
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		// Verifica si la textura fue cargada antes y si es asi, continua con la siguiente iteracion: en caso controaio se salta la carga
		GLboolean skip = false;
		for (GLuint j = 0; j < textures.size(); j++) {
			if (textures[j].second->getFileName() == str.C_Str()) {
				textures.push_back(textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			std::string filename = std::string(str.C_Str());
			filename = this->path + '/' + filename;
			textures.push_back(std::make_pair(type, std::make_shared<Texture2D>(filename, true)));
		}
	}
}

void Mesh::extractBoneWeight(const aiMesh* mesh, std::map<std::string, BoneInfo>& boneInfoMap) {
	std::vector<VertexBoneData> bones;
	u_int numBones = boneInfoMap.size();
	if(mesh->mNumBones > 0){
		bones.resize(mesh->mNumVertices);
		for (u_int i = 0; i < mesh->mNumBones; i++) {
			int boneIndex = -1;
			std::string boneName(mesh->mBones[i]->mName.data);

			if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
				BoneInfo boneInfo;
				boneInfo.id = numBones;
				boneInfo.boneOffset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[i]->mOffsetMatrix);
				boneInfoMap[boneName] = boneInfo;
				boneIndex = numBones++;
			} else
				boneIndex = boneInfoMap[boneName].id;
				
			assert(boneIndex != -1);

			for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
				unsigned int vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
				float weight = mesh->mBones[i]->mWeights[j].mWeight;
				bones[vertexID].AddBoneData(boneIndex, weight);
			}
		}

		glBindVertexArray(this->VAO);
		glGenBuffers(1, &this->VBOBones);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBOBones);
		glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0],
		GL_STATIC_DRAW);

		glVertexAttribIPointer(4, 4, GL_INT, sizeof(VertexBoneData),
				(const GLvoid*) 0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData),
				(const GLvoid*) 16);
		glEnableVertexAttribArray(5);

		glBindVertexArray(0);
	}

}

void Mesh::readHierarchyData(AssimpNodeData& dest, const aiNode* src) {
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

const aiNodeAnim* Mesh::findNodeAnim(const aiAnimation *pAnimation,
		const std::string nodeName) {
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim *pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == nodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}

void Mesh::calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform, 
	const aiAnimation *pAnimation, float animationTime, std::vector<glm::mat4>& finalBoneMatrices, 
	std::map<std::string, Bone> &bones){
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	auto boneIt = bones.find(nodeName);

	const aiNodeAnim *pNodeAnim = findNodeAnim(pAnimation, nodeName);
	if (pNodeAnim && boneIt != bones.end()) {
		boneIt->second.update(pNodeAnim, animationTime);
		nodeTransform = boneIt->second.getLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].boneOffset;
		//std::cout << "Actualizando el indice :=> " << index << std::endl;
		finalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->children.size(); i++)
		calculateBoneTransform(&node->children[i], globalTransformation, 
			pAnimation, animationTime, finalBoneMatrices, bones);
}