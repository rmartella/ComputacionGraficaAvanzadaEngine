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

#include "Texture.hpp"
#include <string>

class DLL_PUBLIC Texture2D : public Texture {

public:
	Texture2D(const std::string& fileName, bool load = true);
	void loadImage( bool flipImage = false);
	void freeImage();

	std::string getFileName() {
		return fileName;
	}

	int getWidth(){
		return this->imageWidth;
	}

	int getHeight(){
		return this->imageHeight;
	}

	int getChannels(){
		return this->channels;
	}

	unsigned char * getData(){
		return this->data;
	}

private:
	virtual void load() override;

protected:
	std::string fileName;
	int imageWidth = 0;
	int imageHeight = 0;
	int channels = 0;
	unsigned char * data;
};
