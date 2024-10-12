#include "Headers/Texture2D.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "Headers/stb_image.h"

Texture2D::Texture2D(const std::string& fileName, bool load) : 
	Texture(GL_TEXTURE_2D), fileName(fileName){
	if(load)
		this->load();
}

void Texture2D::loadImage(bool flipImage) {
	const char * filename = fileName.c_str();
	this->data = stbi_load(filename, &this->imageWidth, &this->imageHeight, &this->channels, 0);
	std::cout << "La textura :=>" << this->fileName << " tiene " << this->channels << " Canales" << std::endl;
}

void Texture2D::freeImage() {
	// Unload the 32-bit colour bitmap
	// Se desecha los datos de la textura
	stbi_image_free(this->data);
}

void Texture2D::load() {
	this->loadImage();

	if(this->data){
		// Se genera un buffer para textura en la GPU
		glGenTextures(1, &idTextureObj);
		glBindTexture(textureTarget, idTextureObj);

		// Se indica el tipo de interpolacion para ajustar la imagen que se cargo a la GPU
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Se envian los datos de la textura
		glTexImage2D(GL_TEXTURE_2D, // Tipo de textura
				0, // Niveles del Mipmap
				channels == 3 ? GL_RGB : GL_RGBA, //Formato intero, RGBA
				imageWidth, // Ancho de la textura
				imageHeight, // Ancho de la textura
				0, // Borde de la textura
				channels == 3 ? GL_RGB : GL_RGBA, // Formato que se maneja la textura
				GL_UNSIGNED_BYTE, // Tipo de datos de la textura
				this->data); // Imagen que se usa para esta textura

		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unload the 32-bit colour bitmap
		// Se desecha los datos de la textura
		freeImage();
	}

	// Desenlazamos la textura
	glBindTexture(textureTarget, 0);
}
