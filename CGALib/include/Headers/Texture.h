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

#include <GL/glew.h>

class DLL_PUBLIC Texture {
public:

	Texture(GLenum textureTarget){
    this->textureTarget = textureTarget;
  }
	
  ~Texture(){
    glBindTexture(textureTarget, 0);
    if(idTextureObj)
      glDeleteTextures(1, &idTextureObj);
  }
  
  void bind(GLenum textureUnit){
    // Se activan la unidade de textura para el objeto de textura con el que fue creado este objeto
	  glActiveTexture(textureUnit);
	  glBindTexture(textureTarget, idTextureObj);
  }
	
  virtual void load() = 0;

protected:
	GLuint idTextureObj;
	GLenum textureTarget;
};
