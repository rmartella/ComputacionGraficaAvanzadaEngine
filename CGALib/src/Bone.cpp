/*
 * Bone.cpp
 *
 *  Created on: 02/12/2016
 *      Author: rey
 */

#include <map>

#include "Headers/Bone.h"
#include "Headers/assimp_glm_helpers.h"

Bone::Bone(const std::string& name, int id) : m_Name(name), /*m_Id(id),*/ m_LocalTransform(1.0) {
}

void Bone::update(const aiNodeAnim *pNodeAnim, float animationTime){
	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		calcInterpolatedScaling(Scaling, animationTime, pNodeAnim);
		glm::mat4 ScalingM = glm::scale(glm::mat4(1.0),
				glm::vec3(Scaling.x, Scaling.y, Scaling.z));

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		calcInterpolatedRotation(RotationQ, animationTime, pNodeAnim);
		aiMatrix4x4 RotationMA = aiMatrix4x4(RotationQ.GetMatrix());
		glm::mat4 RotationM = AssimpGLMHelpers::ConvertMatrixToGLMFormat(RotationMA);

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		calcInterpolatedPosition(Translation, animationTime, pNodeAnim);
		glm::mat4 TranslationM = glm::translate(glm::mat4(1.0),
				glm::vec3(Translation.x, Translation.y, Translation.z));

		// Combine the above transformations
		m_LocalTransform = TranslationM * RotationM * ScalingM;
	}
}

unsigned int Bone::findPosition(float AnimationTime, const aiNodeAnim *pNodeAnim) {
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float) pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
	return 0;
}

unsigned int Bone::findRotation(float AnimationTime, const aiNodeAnim *pNodeAnim) {
	assert(pNodeAnim->mNumRotationKeys > 0);
	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float) pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
	return 0;
}

unsigned int Bone::findScaling(float AnimationTime, const aiNodeAnim *pNodeAnim) {
	assert(pNodeAnim->mNumScalingKeys > 0);
	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float) pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}
	assert(0);
	return 0;
}

void Bone::calcInterpolatedPosition(aiVector3D &Out, float AnimationTime,
		const aiNodeAnim *pNodeAnim) {
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = findPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float) (pNodeAnim->mPositionKeys[NextPositionIndex].mTime
			- pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime
			- (float) pNodeAnim->mPositionKeys[PositionIndex].mTime)
			/ DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D &Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D &End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void Bone::calcInterpolatedRotation(aiQuaternion &Out, float AnimationTime,
		const aiNodeAnim *pNodeAnim) {
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = findRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float) (pNodeAnim->mRotationKeys[NextRotationIndex].mTime
			- pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime
			- (float) pNodeAnim->mRotationKeys[RotationIndex].mTime)
			/ DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion &StartRotationQ =
			pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion &EndRotationQ =
			pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void Bone::calcInterpolatedScaling(aiVector3D &Out, float AnimationTime,
		const aiNodeAnim *pNodeAnim) {
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = findScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float) (pNodeAnim->mScalingKeys[NextScalingIndex].mTime
			- pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime
			- (float) pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D &Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D &End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}