#include "Headers/Quad.hpp"

Quad::Quad(Shader* shader_ptr) : SimpleModel(shader_ptr) {
	std::vector<GLuint> index = {
		0, 1, 2, 0, 2, 3
	};
	std::vector<Vertex> vertexArray = {
		Vertex(glm::vec3(0.5, 0.5, 0.0), glm::vec2(1.0f, 1.0f), glm::vec3(0.0, 0.0, 1.0)),
		Vertex(glm::vec3(-0.5, 0.5, 0.0), glm::vec2(0.0f, 1.0f), glm::vec3(0.0, 0.0, 1.0)),
		Vertex(glm::vec3(-0.5, -0.5, 0.0), glm::vec2(0.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0)),
		Vertex(glm::vec3(0.5, -0.5, 0.0), glm::vec2(1.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0)),
	};

	this->init(vertexArray, index);
}

