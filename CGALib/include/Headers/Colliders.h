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

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <iostream>

#include "Renderable.h"

enum TYPE_COLLIDER {
	SPHERE, BOX, CYLINDER, NONE
};

class DLL_PUBLIC Collider {
public:
	Collider(TYPE_COLLIDER type, glm::vec3 center) : type(type), center(center) {}
	Collider(TYPE_COLLIDER type) : type(type) {}

	void virtual updateCollider(const glm::vec3 mins, const glm::vec3 maxs) = 0;
	bool virtual testCollision(Collider* collider) = 0;
	void virtual updateLogicCollider(Collider* baseCollider, glm::mat4 modelMatrix = glm::mat4(1.0)) = 0;

	glm::vec3& getCenter() { return center; }
	Renderable* getRenderableCollider() { return renderableCollider; }
	void setRenderableCollider(Renderable* renderableCollider) { this->renderableCollider = renderableCollider; }

	TYPE_COLLIDER& getTypeCollider() { return type; }

protected:
	void virtual updateCollider(glm::mat4 modelMatrix) = 0;

	TYPE_COLLIDER type;
	glm::vec3 center;
private:
	Renderable* renderableCollider = nullptr;
};

class DLL_PUBLIC SBBCollider : public Collider {
public:

	SBBCollider() : Collider(SPHERE) {}
	
	SBBCollider(glm::vec3 c, float ratio) : Collider(SPHERE, c){
		this->ratio = ratio;
	}

	SBBCollider(const SBBCollider& other) : Collider(SPHERE, other.center){
		this->ratio = other.ratio;
	}

	void updateCollider(glm::mat4 modelMatrix) {
		this->ratio = glm::length(glm::vec3(modelMatrix[0]));
		this->center = modelMatrix[3];
	}

	void updateCollider(const glm::vec3 mins, const glm::vec3 maxs) override {
		this->center = glm::vec3((mins.x + maxs.x) / 2.0f,
			(mins.y + maxs.y) / 2.0f,
			(mins.z + maxs.z) / 2.0f);
		float dx = maxs.x - mins.x;
		float dy = maxs.y - mins.y;
		float dz = maxs.z - mins.z;
		this->ratio = std::max(std::max(dx, dy), dz) / 2;
		/*this->ratio = sqrt(
			pow(mins.x - maxs.x, 2) + pow(mins.y - maxs.y, 2) + pow(mins.z - maxs.z, 2)) / 2.0f;*/
	}

	bool testCollision(Collider* collider) override {
		SBBCollider* a = this;
		if(collider->getTypeCollider() == BOX){
			collider->testCollision(a);
		}
		else if(collider->getTypeCollider() == SPHERE){
			SBBCollider* b = dynamic_cast<SBBCollider*>(collider);
			float d = glm::distance(a->getCenter(), b->getCenter());
			if (d <= (a->getRatio() + b->getRatio()))
				return true;
		}
		return false;
	}

	void updateLogicCollider(Collider* baseCollider, glm::mat4 modelMatrix) override {
		SBBCollider* sbbColliderBase = dynamic_cast<SBBCollider*>(baseCollider);
		glm::mat4 modelMatrixCollider = glm::mat4(modelMatrix);
		modelMatrixCollider = glm::translate(modelMatrixCollider, sbbColliderBase->getCenter());
		float scaleX = glm::length(glm::vec3(modelMatrixCollider[0])); // Se asume un scalamiento isomorfico si no ya no es una esfera
		this->center = modelMatrixCollider[3];
		this->ratio = sbbColliderBase->getRatio() * scaleX;
		if(this->getRenderableCollider()){
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, center);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(ratio * 2.0f));
			this->getRenderableCollider()->getModelMatrix() = matrixCollider;
			this->getRenderableCollider()->render();
		}
	}

	float& getRatio() { return ratio; }

private:
	float ratio;
};

class DLL_PUBLIC OBBCollider : public Collider {
public:

	OBBCollider() : Collider(BOX) {}

	OBBCollider(glm::vec3 center, glm::vec3 halfways, glm::quat orientation) : Collider(BOX, center) {
		this->halfways = halfways;
		this->orientation = orientation;
	}

	OBBCollider(const OBBCollider& other) : Collider(BOX, other.center){
		this->halfways = other.halfways;
		this->orientation = other.orientation;
	}

	glm::vec3& getHalfways() { return halfways; }

	void updateCollider(glm::mat4 modelMatrix) {
		float sx = glm::length(glm::vec3(modelMatrix[0]));
		float sy = glm::length(glm::vec3(modelMatrix[1]));
		float sz = glm::length(glm::vec3(modelMatrix[2]));
		this->halfways = this->halfways * glm::vec3(sx, sy, sz);
		this->orientation = glm::quat_cast(modelMatrix);
		this->center = modelMatrix[3];
	}

	void updateCollider(const glm::vec3 mins, const glm::vec3 maxs) override {
		this->center = glm::vec3((mins.x + maxs.x) / 2.0f,
			(mins.y + maxs.y) / 2.0f,
			(mins.z + maxs.z) / 2.0f);
		this->halfways = (maxs - mins) / 2.0f;
	}

	bool testCollision(Collider* collider) override {
		OBBCollider* a = this;
		if(collider->getTypeCollider() == BOX){
			OBBCollider* b = dynamic_cast<OBBCollider*>(collider);
			float EPSILON = 0.0001;
			float ra, rb;
			glm::mat3 R = glm::mat4(0.0), AbsR = glm::mat4(0.0);
			// compute the matrix axis from the quaternions
			glm::mat3 matA = glm::mat3(a->orientation);
			glm::mat3 matB = glm::mat3(b->orientation);
			// Compute rotation matrix expressing b in a's coordinate frames
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					R[i][j] = glm::dot(matA[i], matB[j]);
			// Compute translation vector
			glm::vec3 t = b->center - a->center;
			// Bring translation into a’s coordinate frame
			t = glm::vec3(glm::dot(t, matA[0]), glm::dot(t, matA[1]), glm::dot(t, matA[2]));
			// Compute common subexpressions. Add in an epsilon term to
			// counteract arithmetic errors when two edges are parallel and
			// their cross product is (near) null (see text for details)
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					AbsR[i][j] = fabs(R[i][j]) + EPSILON;
			// Test axes L = A0, L = A1, L = A2
			for (int i = 0; i < 3; i++) {
				ra = a->halfways[i];
				rb = b->halfways[0] * AbsR[i][0] + b->halfways[1] * AbsR[i][1] + b->halfways[2] * AbsR[i][2];
				if (fabs(t[i]) > ra + rb) return false;
			}
			// Test axes L = B0, L = B1, L = B2
			for (int i = 0; i < 3; i++) {
				ra = a->halfways[0] * AbsR[0][i] + a->halfways[1] * AbsR[1][i] + a->halfways[2] * AbsR[2][i];
				rb = b->halfways[i];
				if (fabs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return false;
			}
			// Test axis L = A0 x B0
			ra = a->halfways[1] * AbsR[2][0] + a->halfways[2] * AbsR[1][0];
			rb = b->halfways[1] * AbsR[0][2] + b->halfways[2] * AbsR[0][1];
			if(fabs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return false;

			// Test axis L = A0 x B1
			ra = a->halfways[1] * AbsR[2][1] + a->halfways[2] * AbsR[1][1];
			rb = b->halfways[0] * AbsR[0][2] + b->halfways[2] * AbsR[0][0];
			if(fabs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return false;

			// Test axis L = A0 x B2
			ra = a->halfways[1] * AbsR[2][2] + a->halfways[2] * AbsR[1][2];
			rb = b->halfways[0] * AbsR[0][1] + b->halfways[1] * AbsR[0][0];
			if(fabs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return false;

			// Test axis L = A1 x B0
			ra = a->halfways[0] * AbsR[2][0] + a->halfways[2] * AbsR[0][0];
			rb = b->halfways[1] * AbsR[1][2] + b->halfways[2] * AbsR[1][1];
			if(fabs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return false;

			// Test axis L = A1 x B1
			ra = a->halfways[0] * AbsR[2][1] + a->halfways[2] * AbsR[0][1];
			rb = b->halfways[0] * AbsR[1][2] + b->halfways[2] * AbsR[1][0];
			if(fabs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return false;

			// Test axis L = A1 x B2
			ra = a->halfways[0] * AbsR[2][2] + a->halfways[2] * AbsR[0][2];
			rb = b->halfways[0] * AbsR[1][1] + b->halfways[1] * AbsR[1][0];
			if(fabs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return false;

			// Test axis L = A2 x B0
			ra = a->halfways[0] * AbsR[1][0] + a->halfways[1] * AbsR[0][0];
			rb = b->halfways[1] * AbsR[2][2] + b->halfways[2] * AbsR[2][1];
			if(fabs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return false;

			// Test axis L = A2 x B1
			ra = a->halfways[0] * AbsR[1][1] + a->halfways[1] * AbsR[0][1];
			rb = b->halfways[0] * AbsR[2][2] + b->halfways[2] * AbsR[2][0];
			if(fabs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return false;

			// Test axis L = A2 x B2
			ra = a->halfways[0] * AbsR[1][2] + a->halfways[1] * AbsR[0][2];
			rb = b->halfways[0] * AbsR[2][1] + b->halfways[1] * AbsR[2][0];
			if(fabs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return false;

			return true;
		}
		else if(collider->getTypeCollider() == SPHERE){
			SBBCollider* b = dynamic_cast<SBBCollider*>(collider);
			float d = 0;
			glm::quat qinv = glm::inverse(a->orientation);
			glm::vec3 centerB = qinv * glm::vec4(b->getCenter(), 1.0);
			glm::vec3 centerA = qinv * glm::vec4(a->getCenter(), 1.0);
			glm::vec3 mins = a->getCenter() - a->halfways;
			glm::vec3 maxs = a->getCenter() + a->halfways;
			if (b->getCenter()[0] >= mins[0] && b->getCenter()[0] <= maxs[0]
					&& b->getCenter()[1] >= mins[1] && b->getCenter()[1] <= maxs[1]
					&& b->getCenter()[2] >= mins[2] && b->getCenter()[2] <= maxs[2])
				return true;
			for (int i = 0; i < 3; i++){
				if(b->getCenter()[i] < mins[i])
					d += (b->getCenter()[i] - mins[i]) * (b->getCenter()[i] - mins[i]);
				else if(b->getCenter()[i] > maxs[i])
					d += (b->getCenter()[i] - maxs[i]) * (b->getCenter()[i] - maxs[i]);
			}
			if(d <= b->getRatio() * b->getRatio())
				return true;
			return false;
		}

		return false;
	}

	void updateLogicCollider(Collider* baseCollider, glm::mat4 modelMatrix) override {
		OBBCollider* obbColliderBase = dynamic_cast<OBBCollider*>(baseCollider);
		
		glm::mat4 modelMatrixCollider = glm::mat4(modelMatrix);
		float scaleX = glm::length(glm::vec3(modelMatrixCollider[0]));
		float scaleY = glm::length(glm::vec3(modelMatrixCollider[1]));
		float scaleZ = glm::length(glm::vec3(modelMatrixCollider[2]));

		modelMatrixCollider[0] = glm::vec4(glm::normalize(glm::vec3(modelMatrixCollider[0])), 0.0f);
		modelMatrixCollider[1] = glm::vec4(glm::normalize(glm::vec3(modelMatrixCollider[1])), 0.0f);
		modelMatrixCollider[2] = glm::vec4(glm::normalize(glm::vec3(modelMatrixCollider[2])), 0.0f);

		// Set the orientation of collider before doing the scale
		glm::quat ori = glm::quat_cast(modelMatrixCollider);
		
		modelMatrixCollider = glm::scale(modelMatrixCollider, glm::vec3(scaleX, scaleY, scaleZ));
		modelMatrixCollider = glm::translate(modelMatrixCollider, obbColliderBase->getCenter());
		
		this->halfways = obbColliderBase->getHalfways() * glm::vec3(scaleX, scaleY, scaleZ);
		this->center = modelMatrixCollider[3];
		this->orientation = ori;

		if(this->getRenderableCollider()){
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, this->center);
			matrixCollider = matrixCollider * glm::mat4(orientation);
			matrixCollider = glm::scale(matrixCollider, this->halfways * 2.0f);
			this->getRenderableCollider()->getModelMatrix() = matrixCollider;
			this->getRenderableCollider()->render();
		}
	}

private:
	glm::vec3 halfways;
	glm::quat orientation;
};