#ifndef THIRDPERSONCAMERA_H
#define THIRDPERSONCAMERA_H

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

#include "Camera.h"
#include "AbstractModel.h"

class DLL_PUBLIC ThirdPersonCamera: public Camera
{
public:
    ThirdPersonCamera(float distanceFromTarget = 7.0);
    void mouseMoveCamera(float xoffset, float yoffset, float dt);
    void scrollMoveCamera(float soffset, float dt);
    void setModelTarget(AbstractModel* modelTarget) { this->modelTarget = modelTarget; }
    void setDistanceFromTarget(float distanceFromTarget) {
		this->distanceFromTarget = distanceFromTarget;
	}

protected:
    void updateCamera();
    
private:
    float distanceFromTarget;
    float angleAroundTarget;
    AbstractModel* modelTarget = nullptr;
};

#endif // THIRDPERSONCAMERA_H
