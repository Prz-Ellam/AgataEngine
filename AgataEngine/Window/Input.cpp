#include "Input.h"

namespace Agata {

	void Input::setCursorMode(Window* window, uint32_t mode) {

		glfwSetInputMode(window->getHandler(), GLFW_CURSOR, mode);

	}

	uint32_t Input::getCursorMode(Window* window) {
		
		return glfwGetInputMode(window->getHandler(), GLFW_CURSOR);

	}

}