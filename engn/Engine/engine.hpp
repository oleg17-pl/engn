#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <memory>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Scene/scene.hpp"
#include "Objects/Camera/camera.hpp"
#include "Shader/shader.hpp"

struct WindowConfig {
	const int width, height;
	const char *name;
};

class Engine {
private:
	static std::shared_ptr<Engine> instance;

	bool initialized, started;
	float delta_time;
	int current_width, current_height;

	GLFWwindow *window;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Shader> shader;

	bool initGLFW();
	bool initGLAD();
	bool initWindow(const WindowConfig &wconf);

	static void onFramebufferSizeChanged(GLFWwindow *const window, const int width, const int height);
	static void onCursorPositionChanged(GLFWwindow *const window, const double xpos, const double ypos);
	void framebuffer_size_callback(const int width, const int height);
	void cursor_position_callback(const double xpos, const double ypos);

	void processInput();
	void updateDeltaTime();
	void loop();
public:
	static std::shared_ptr<Engine> getInstance();
	bool init(const WindowConfig &wconf);

	void setCamera(const std::shared_ptr<Camera> &camera);
	void setScene(const std::shared_ptr<Scene> &scene);
	void setShader(const std::shared_ptr<Shader> &shader);

	std::shared_ptr<Camera> getCamera();
	std::shared_ptr<Scene> getScene();
	std::shared_ptr<Shader> getShader();

	void start();

	float getScreenAspect() const;
	float getDeltaTime() const;
	int getFPS() const;

	Engine();
	~Engine();
};

#endif // !ENGINE_HPP