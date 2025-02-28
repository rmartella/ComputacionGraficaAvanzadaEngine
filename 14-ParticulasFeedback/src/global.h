#include <memory>
#include "Headers/hmodels/DarthVader.h" 
#include "Headers/Model.h"
#include "Headers/KeyFrameAnimator.h"

extern Shader shader;
//Shader con skybox
extern Shader shaderSkybox;
// Shader para dibujar un objeto con solo textura
extern Shader shaderTexture;
//Shader con multiples luces
extern Shader shaderMulLighting;
//Shader para el terreno
extern Shader shaderTerrain;
//Shader para dibujar el buffer de profunidad
extern Shader shaderDepth;
// Shader para visualizar el buffer de profundidad
extern Shader shaderViewDepth;

extern std::shared_ptr<Sphere> esfera1;
extern std::shared_ptr<Sphere> esfera2;
extern std::shared_ptr<Box> box1;
extern std::shared_ptr<Sphere> sphereCollider;
extern std::shared_ptr<Box> boxCollider;
extern std::shared_ptr<Quad> boxScreen;

extern std::shared_ptr<Model> modelRock;
extern std::shared_ptr<Model> modelAircraft;
extern std::shared_ptr<Model> modelLamp1;
extern std::shared_ptr<Model> modelLamp2;
extern std::shared_ptr<Model> modelLampPost2;

extern std::shared_ptr<Model> mayowModelAnimate;
extern std::shared_ptr<Model> cyborgModelAnimate;
extern std::shared_ptr<Model> cowboyModelAnimate;
extern std::shared_ptr<Model> guardianModelAnimate;

extern std::shared_ptr<Eclipse> eclipse;
extern std::shared_ptr<Lambo> lambo;
extern std::shared_ptr<Heli> heli;
extern std::shared_ptr<DarthVader> darthVader;

extern std::shared_ptr<KeyFrameAnimator> darthAnimation;
extern std::shared_ptr<KeyFrameAnimator> darthAnimation2;

extern std::shared_ptr<BlendMapTerrain> terrain;

// Lamps position
extern std::vector<std::pair<glm::vec3, float>> lamp1;
extern std::vector<std::pair<glm::vec3, float>> lamp2;

extern std::shared_ptr<SpotLight> spotLight;

extern std::shared_ptr<KeyFrameAnimator> darthAnimation;
extern std::shared_ptr<KeyFrameAnimator> darthAnimation2;
extern bool availableSave;

extern std::shared_ptr<SpotLight> spotLight;

extern std::shared_ptr<FontTypeRendering::FontTypeRendering> modelText;

extern std::shared_ptr<SkyBoxSphere> skyBoxSphere;
extern std::shared_ptr<Texture2D> textureLanding;
extern std::shared_ptr<Texture2D> textureIntro1;
extern std::shared_ptr<Texture2D> textureIntro2;
extern std::shared_ptr<Texture2D> textureScreen;
extern Texture2D* textureIntroActive;
extern bool iniciaPartida, presionarOpcion;