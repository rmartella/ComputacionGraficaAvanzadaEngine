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

#include "Colliders.h"

class DLL_PUBLIC ObjectCollider {
public:
	ObjectCollider(TYPE_COLLIDER typeCollider) : typeCollider(typeCollider) {}
	~ObjectCollider(){
		if(collider != nullptr)
			delete collider;
    if(initCollider != nullptr)
			delete initCollider;
	}

	Collider* getCollider() { return collider; }
    Collider* getInitCollider() { return initCollider; }

protected:
	TYPE_COLLIDER typeCollider;
	Collider* collider = nullptr;
    Collider* initCollider = nullptr;
	
    virtual void updateCollider(glm::mat4 modelMatrix) {
        collider->updateLogicCollider(initCollider, modelMatrix);
    }
};


