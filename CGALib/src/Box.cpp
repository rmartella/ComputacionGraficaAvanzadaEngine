#include "Headers/Box.h"

Box::Box(Shader* shader_ptr, BaseTerrain *terrain) : 
	SimpleModel(shader_ptr), ObjectCollider(BOX), TerrainAnimator(terrain) {
	std::vector<GLuint> index = {
		0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23
	};
	std::vector<Vertex> vertexArray = {
		Vertex(glm::vec3(0.5, 0.5, 0.5), glm::vec2(1.0f, 1.0f), glm::vec3(0.0, 0.0, 1.0)),
		Vertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0.0f, 1.0f), glm::vec3(0.0, 0.0, 1.0)),
		Vertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0)),
		Vertex(glm::vec3(0.5, -0.5, 0.5), glm::vec2(1.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0)),
		Vertex(glm::vec3(0.5, 0.5, 0.5), glm::vec2(0.0f, 1.0f), glm::vec3(1.0, 0.0, 0.0)),
		Vertex(glm::vec3(0.5, -0.5, 0.5), glm::vec2(0.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0)),
		Vertex(glm::vec3(0.5, -0.5, -0.5), glm::vec2(1.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0)),
		Vertex(glm::vec3(0.5, 0.5, -0.5), glm::vec2(1.0f, 1.0f), glm::vec3(1.0, 0.0, 0.0)),
		Vertex(glm::vec3(0.5, 0.5, -0.5), glm::vec2(0.0f, 1.0f), glm::vec3(0.0, 0.0, -1.0)),
		Vertex(glm::vec3(0.5, -0.5, -0.5), glm::vec2(0.0f, 0.0f), glm::vec3(0.0, 0.0, -1.0)),
		Vertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(1.0f, 0.0f), glm::vec3(0.0, 0.0, -1.0)),
		Vertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec2(1.0f, 1.0f), glm::vec3(0.0, 0.0, -1.0)),
		Vertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0, 0.0, 0.0)),
		Vertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0, 0.0, 0.0)),
		Vertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0, 0.0, 0.0)),
		Vertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0, 0.0, 0.0)),
		Vertex(glm::vec3(0.5, 0.5, 0.5), glm::vec2(1.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0)),
		Vertex(glm::vec3(0.5, 0.5, -0.5), glm::vec2(1.0f, 1.0f), glm::vec3(0.0, 1.0, 0.0)),
		Vertex(glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0.0f, 1.0f), glm::vec3(0.0, 1.0, 0.0)),
		Vertex(glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0)),
		Vertex(glm::vec3(0.5, -0.5, 0.5), glm::vec2(1.0f, 1.0f), glm::vec3(0.0, -1.0, 0.0)),
		Vertex(glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0.0f, 1.0f), glm::vec3(0.0, -1.0, 0.0)),
		Vertex(glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0.0f, 0.0f), glm::vec3(0.0, -1.0, 0.0)),
		Vertex(glm::vec3(0.5, -0.5, -0.5), glm::vec2(1.0f, 0.0f), glm::vec3(0.0, -1.0, 0.0)),
	};

	this->init(vertexArray, index);

	initCollider = new OBBCollider(glm::vec3(0.0), glm::vec3(0.5), glm::quat(0.0, 0.0, 0.0, 1));
	collider = new OBBCollider(glm::vec3(0.0), glm::vec3(0.5), glm::quat(0.0, 0.0, 0.0, 1));
}

void Box::render() {
	animate(modelMatrix);
	SimpleModel::render();
	ObjectCollider::updateCollider(glm::scale(modelMatrix, glm::vec3(scale)));
}

