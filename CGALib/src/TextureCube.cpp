#include "Headers/TextureCube.hpp"
#include <iostream>
#include "Headers/stb_image.h"

TextureCube::TextureCube(const std::vector<std::pair<GLenum, std::string>>& cubeFaces) : Texture(GL_TEXTURE_CUBE_MAP), cubeFaces(cubeFaces){
	this->load();
}

void TextureCube::load() {
	glGenTextures(1, &idTextureObj);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, idTextureObj);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < cubeFaces.size(); i++) {
		Texture2D skyboxTexture(cubeFaces[i].second, false);
		skyboxTexture.loadImage();
		if (skyboxTexture.getData()) {
			glTexImage2D(cubeFaces[i].first, 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
			skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
			skyboxTexture.freeImage();
		} else
			std::cout << "Failed to load texture" << std::endl;
	}
}
