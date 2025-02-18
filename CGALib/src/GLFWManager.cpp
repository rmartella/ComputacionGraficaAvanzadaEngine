
#include "Headers/GLFWManager.hpp"
#include "Headers/GLFWInputManager.hpp"

int WindowManager::screenWidth = 800;
int WindowManager::screenHeight = 700;
InputManager* WindowManager::inputManager = new GLFWInputManager();
double GLFWManager::deltaTime;

GLFWManager::GLFWManager() : window(nullptr) {
}

GLFWManager::~GLFWManager() {
	destroy();
}

bool GLFWManager::init(int width, int height, std::string strTitle,
		bool bFullScreen) {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	this->screenWidth = width;
	this->screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		return false;
	}

    glViewport(0, 0, screenWidth, screenHeight);

	return true;
}

bool GLFWManager::processInput(bool continueApplication = true, const std::vector<std::function<void(int)>>& callbacks = {}) {
	if (glfwWindowShouldClose(window) != 0
			||  inputManager->getStatusKey(GLFW_KEY_ESCAPE)) {
		return false;
	}

	for (const auto& callback : callbacks)
		callback(inputManager->getModelSelected());
	
    // TODO Movimientos de la camara o eventos de InputManager
	inputManager->controller(TimeManager::Instance().DeltaTime);
	
	glfwPollEvents();
	return continueApplication;
}

void GLFWManager::swapTheBuffers() {
	glfwSwapBuffers(window);
}

void GLFWManager::destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void GLFWManager::reshapeCallback(GLFWwindow* Window, int widthRes,
		int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void GLFWManager::keyCallback(GLFWwindow* window, int key, int scancode,
		int action, int mode) {
	inputManager->keyPressed(key, deltaTime, action);
}

void GLFWManager::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	inputManager->mouseMoved(xpos, ypos);
}

void GLFWManager::mouseButtonCallback(GLFWwindow* window, int button, int state,
		int mod) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	inputManager->mouseClicked(button, state);
}

void GLFWManager::mouseScrollCallback(GLFWwindow* window, double xoffset,
		double yoffset) {
	inputManager->mouseScroll(yoffset);
}