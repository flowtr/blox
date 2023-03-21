#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace bobux::engine {
class Engine {
public:
	Engine() {
		glfwSetErrorCallback([](int error, const char* description) {
			throw std::runtime_error(description);
		});

		if (!glfwInit())
				throw std::runtime_error("Failed to initialize GLFW");
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		window = glfwCreateWindow(640, 480, "Bobux", NULL, NULL);
		if (!window)
		{
			throw std::runtime_error("Failed to create GLFW window");
		}

		glfwMakeContextCurrent(window);
		gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	}

	auto should_run() -> bool {
		return !glfwWindowShouldClose(window);
	}	

	auto handle_post_draw() {
		glfwSwapBuffers(window);
	}
private:
	GLFWwindow* window;
};
}

