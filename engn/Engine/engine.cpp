#include "engine.hpp"

std::shared_ptr<Engine> Engine::instance{};

std::shared_ptr<Engine> Engine::getInstance() {
	if (!instance) {
		instance = std::make_shared<Engine>();
	}
	return instance;
}

bool Engine::initGLFW() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	std::cout << "GLFW initialized" << std::endl;
	return true;
}

bool Engine::initGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	std::cout << "GLAD initialized" << std::endl;
	return true;
}

void Engine::framebuffer_size_callback(const int width, const int height) {
	glViewport(0, 0, width, height);
	current_width = width;
	current_height = height;
}

void Engine::cursor_position_callback(const double xpos, const double ypos) {
	if (!camera) {
		return;
	}
	static glm::vec2 last_cursor_pos = glm::vec2{ 0.0f };
	static bool first_mouse = true;
	glm::vec2 current_cursor_pos(xpos, ypos);
	if (first_mouse) {
		last_cursor_pos = current_cursor_pos;
		first_mouse = false;
	}
	glm::vec2 offset{ 0.0f };
	offset.x = last_cursor_pos.x - current_cursor_pos.x;
	offset.y = current_cursor_pos.y - last_cursor_pos.y;
	last_cursor_pos = current_cursor_pos;
	camera->processMouseMovement(offset);
}

bool Engine::initWindow(const WindowConfig &wconf) {
	window = glfwCreateWindow(wconf.width, wconf.height, wconf.name, NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create window" << std::endl;
		window = nullptr;
		return false;
	}
	std::cout << "Window created" << std::endl;
	glfwMakeContextCurrent(window);
	glfwGetWindowSize(window, &current_width, &current_height);
	glfwSetFramebufferSizeCallback(window, onFramebufferSizeChanged);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, onCursorPositionChanged);
	return true;
}

void Engine::onFramebufferSizeChanged(GLFWwindow *const window, const int width, const int height) {
	if (instance) {
		instance->framebuffer_size_callback(width, height);
	}
}

void Engine::onCursorPositionChanged(GLFWwindow *const window, const double xpos, const double ypos) {
	if (instance) {
		instance->cursor_position_callback(xpos, ypos);
	}
}

void Engine::processInput() {
	if (!camera) {
		return;
	}
	GLFWwindow *wndw{ window };
	auto key_pressed{
		[wndw](const int code) -> bool {
			return glfwGetKey(wndw, code) == GLFW_PRESS;
		}
	};
	if (key_pressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
	if (key_pressed(GLFW_KEY_W))
		camera->processKeyboardInput(MoveDirection::FORWARD);
	if (key_pressed(GLFW_KEY_A))
		camera->processKeyboardInput(MoveDirection::LEFT);
	if (key_pressed(GLFW_KEY_S))
		camera->processKeyboardInput(MoveDirection::BACK);
	if (key_pressed(GLFW_KEY_D))
		camera->processKeyboardInput(MoveDirection::RIGHT);
	if (key_pressed(GLFW_KEY_E))
		camera->processKeyboardInput(MoveDirection::UP);
	if (key_pressed(GLFW_KEY_Q))
		camera->processKeyboardInput(MoveDirection::DOWN);
}

void Engine::updateDeltaTime() {
	float current_time{ static_cast<float>(glfwGetTime()) };
	static float last_time{ current_time };
	delta_time = last_time - current_time;
	last_time = current_time;
}

bool Engine::init(const WindowConfig &wconf) {
	bool isglfw{ initGLFW() };
	bool iswindow{ initWindow(wconf) };
	bool isglad{ initGLAD() };
	if (!(isglfw || iswindow || isglad)) {
		if (isglfw) {
			glfwTerminate();
		}
	}
	initialized = true;
	return initialized;
}

void Engine::setCamera(const std::shared_ptr<Camera> &camera) {
	this->camera = camera;
}

void Engine::setScene(const std::shared_ptr<Scene> &scene) {
	this->scene = scene;
}

void Engine::setShader(const std::shared_ptr<Shader> &shader) {
	this->shader = shader;
}

std::shared_ptr<Camera> Engine::getCamera() {
	return camera;
}

std::shared_ptr<Scene> Engine::getScene() {
	return scene;
}

std::shared_ptr<Shader> Engine::getShader() {
	return shader;
}

void Engine::start() {
	if (started) {
		return;
	}
	shader->use();
	glEnable(GL_DEPTH_TEST);
	started = true;
	loop();
}

float Engine::getScreenAspect() const {
	float f_width{ static_cast<float>(current_width) };
	float f_height{ static_cast<float>(current_height) };
	return f_width / f_height;
}

float Engine::getDeltaTime() const {
	return delta_time;
}

int Engine::getFPS() const {
	return static_cast<int>(-(1.0f / delta_time));
}

void Engine::loop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		processInput();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene->draw();
		glfwSwapBuffers(window);
		updateDeltaTime();
	}
}

Engine::Engine()
	: initialized{ false },
	started{ false },
	window{ nullptr },
	current_width{},
	current_height{},
	delta_time{}
{
	if (instance) {
		throw std::exception{ "Engine instance already exists!" };
	}
}

Engine::~Engine() {
	glfwTerminate();
}
