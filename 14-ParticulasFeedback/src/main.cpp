#include "Headers/Scene.h"

int main(int argc, char **argv) {
	Scene escena;
	escena.init(800, 700, "Window GLFW", false);
	escena.run();
	return 1;
}