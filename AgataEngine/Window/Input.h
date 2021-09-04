#ifndef AGATA_INPUT_H
#define AGATA_INPUT_H

#include <GLFW/glfw3.h>
#include "Window.h"

namespace Agata {

	enum CursorMode {
		Disabled = GLFW_CURSOR_DISABLED,
		Normal = GLFW_CURSOR_NORMAL
	};

	class Input {
	public:
		static void setCursorMode(Window* window, uint32_t mode);
		static uint32_t getCursorMode(Window* window);
	};

}

#endif