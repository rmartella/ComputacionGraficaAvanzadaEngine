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

#include <iostream>
#include <vector>
#include <memory>
#define _USE_MATH_DEFINES
#include <cmath>
#include <GL/glew.h>
#include "Shader.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

class DLL_PUBLIC Drawable
{
protected:
	class Vertex {
		public:
			Vertex() {}
			Vertex(glm::vec3 m_pos, glm::vec2 m_tex, glm::vec3 m_normal) : m_pos(m_pos), m_tex(m_tex), m_normal(m_normal) { }

		public:
			glm::vec3 m_pos;
			glm::vec2 m_tex;
			glm::vec3 m_normal;
	};

public:
	Drawable() = default;

	~Drawable(){
		this->destroy();
	}

	void update(std::vector<Vertex> vertexArray) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		GLvoid* p = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		memcpy(p, vertexArray.data(), vertexArray.size() * sizeof(vertexArray[0]));
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	
protected:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	size_t totalIndex = 0;

	void draw() {
		if(totalIndex > 0)
			draw(0, totalIndex);
	}

	void init(std::vector<Vertex> vertexArray, std::vector<GLuint> index) {
		this->totalIndex = index.size();

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(vertexArray[0]), vertexArray.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalIndex * sizeof(index[0]), index.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexArray[0]), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertexArray[0]), (GLvoid*)(sizeof(vertexArray[0].m_pos)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertexArray[0]), (GLvoid*)(sizeof(vertexArray[0].m_pos) + sizeof(vertexArray[0].m_tex)));
		glEnableVertexAttribArray(3);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

private:

	void destroy() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &EBO);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VAO);
	}

	void draw(int indexInit, int indexSize) {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, (GLuint*)(indexInit * sizeof(GLuint)));
		glBindVertexArray(0);
	}
};
