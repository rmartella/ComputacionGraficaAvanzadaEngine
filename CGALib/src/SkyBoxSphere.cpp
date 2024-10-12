#include "Headers/SkyBoxSphere.hpp"

void SkyBoxSphere::render(glm::mat4 parentTrans){
	GLint oldDepthFuncMode;
	GLint oldCullFaceMode;
	// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
	glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
	shader_ptr->setFloat("skybox", 0);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	TextureCube::bind(GL_TEXTURE0);
	Sphere::render(parentTrans);
	glCullFace(oldCullFaceMode);
	glDepthFunc(oldDepthFuncMode);
}