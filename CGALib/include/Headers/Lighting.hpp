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

#include <glm/glm.hpp>
#include "Shader.h"

class DLL_PUBLIC Light {
public:
  Light() = default;
  Light(glm::vec3 ambient, glm::vec3 difuse, glm::vec3 specular) :
    ambient(ambient), difuse(difuse), specular(specular) {}
  Light(const Light& other) : 
    ambient(other.ambient), difuse(other.difuse), specular(other.specular) {}

  glm::vec3& getAmbiente(){ return ambient; }
  glm::vec3& getDifuse(){ return difuse; }
  glm::vec3& getSpecular(){ return specular; }
  
protected:
  glm::vec3 ambient;
  glm::vec3 difuse;
  glm::vec3 specular;
};

class DLL_PUBLIC DirectionalLight : public Light {
public:
  DirectionalLight() = default;
  DirectionalLight(glm::vec3 ambient, glm::vec3 difuse, glm::vec3 specular, glm::vec3 direction):
    Light(ambient, difuse, specular), direction(direction) {}
  DirectionalLight(const Light& light, glm::vec3 direction): Light(light), direction(direction) {}
  
  glm::vec3& getDirection(){ return direction; }

protected:
  DirectionalLight(glm::vec3 direction) : direction(direction) {}
private:
  glm::vec3 direction;
  Light light;
};

class DLL_PUBLIC PointLight : public Light {
public:
  //PointLight() = default;
  PointLight(glm::vec3 ambient, glm::vec3 difuse, glm::vec3 specular, glm::vec3 position, 
    float constant, float linear, float quadratic):
    Light(ambient, difuse, specular), position(position), 
    constant(constant), linear(linear), quadratic(quadratic) {}

  PointLight(const Light& light, glm::vec3 position, float constant, float linear, float quadratic):
    Light(light), position(position), 
    constant(constant), linear(linear), quadratic(quadratic) {}

  glm::vec3& getPosition(){ return position; }
  float& getConstant(){ return constant; }
  float& getLinear(){ return linear; }
  float& getQuadratic(){ return quadratic; }
  
private:
  glm::vec3 position;
  float constant;
  float linear;
  float quadratic;
};

class DLL_PUBLIC SpotLight : public PointLight {
public:
  //SpotLight() = default;
  SpotLight(glm::vec3 ambient, glm::vec3 difuse, glm::vec3 specular, glm::vec3 direction, 
    glm::vec3 position, float constant, float linear, float quadratic, 
    float cutOff, float outerCutOff) : 
    PointLight(ambient, difuse, specular, position, constant, linear, quadratic), direction(direction) {};
  
  glm::vec3& getDirection() { return direction; }
  float& getCutOff() { return cutOff; }
  float& getOuterCutOff() { return outerCutOff; }

private:
  glm::vec3 direction;
  float cutOff;
	float outerCutOff;
};

class DLL_PUBLIC LightManager {
public:
  LightManager() = default;
  void addDirectionalLight(const glm::vec3& ambient, const glm::vec3& difuse, const glm::vec3& specular, const glm::vec3& direction);
  void addDirectionalLight(const Light& light, const glm::vec3& direction);
  void addPointLight(const glm::vec3& ambient, const glm::vec3& difuse, const glm::vec3& specular, const glm::vec3& position, 
    float constant, float linear, float quadratic);
  void addPointLight(const Light& light, const glm::vec3& position, float constant, float linear, float quadratic);
  void addSpotLight(const glm::vec3& ambient, const glm::vec3& difuse, const glm::vec3& specular, const glm::vec3& direction, 
    const glm::vec3& position, float constant, float linear, float quadratic, 
    float cutOff, float outerCutOff);

  void applyLighting(std::vector<Shader*> shaders_ptr); // Llamada para configurar todas las luces en los shaders
private:
    DirectionalLight directionalLights;
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;
};