#include "pch.h"
#include "Window.h"
#include "Log.h"
#include <stb/stb_image.h>

Window::Window(uint32_t width, uint32_t height, const std::string& title) {

	m_WindowData.width = width;
	m_WindowData.height = height;
	m_WindowData.title = title;
	m_WindowData.vSync = false;

	if (!glfwInit()) {
		Log::error("ERROR no se pudo iniciar GLFW\n");
	}

	m_Window = glfwCreateWindow(m_WindowData.width, m_WindowData.height, m_WindowData.title.c_str(), nullptr, nullptr);

	if (!m_Window) {
		Log::error("ERROR no se pudo iniciar la ventana\n");
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_Window);

	glfwSetWindowUserPointer(m_Window, &m_WindowData);
	glfwSetJoystickUserPointer(1, m_Window);

}

Window::Window() {

	m_WindowData.width = 1280;
	m_WindowData.height = 720;
	m_WindowData.title = "Window";
	m_WindowData.vSync = true;

	if (!glfwInit()) {
		Log::error("ERROR no se pudo iniciar GLFW\n");
	}

	m_Window = glfwCreateWindow(m_WindowData.width, m_WindowData.height, m_WindowData.title.c_str(), nullptr, nullptr);

	if (!m_Window) {
		Log::error("ERROR no se pudo iniciar la ventana\n");
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_Window);

	glfwSetWindowUserPointer(m_Window, &m_WindowData);
	glfwSetJoystickUserPointer(1, m_Window);

}

Window::~Window() {

	glfwDestroyWindow(m_Window);
	glfwTerminate();

}

void Window::update() {

	glfwSwapBuffers(m_Window);
	glfwPollEvents();

}

uint32_t Window::getWidth() const {

	return m_WindowData.width;

}

uint32_t Window::getHeight() const {

	return m_WindowData.height;

}

void Window::setVSync(bool enabled) {

	glfwSwapInterval(enabled);
	m_WindowData.vSync = enabled;

}

bool Window::isVSync() const {

	return m_WindowData.vSync;

}

std::string Window::getWindowTitle() const {

	return m_WindowData.title;

}

void Window::setWindowTitle(const std::string& title) {

	glfwSetWindowTitle(m_Window, title.c_str());
	m_WindowData.title = title;

}

void Window::setIcons(const std::string& filePath) {

	GLFWimage icons[2];
	icons[0].pixels = stbi_load(filePath.c_str(), &icons[0].width, &icons[0].height, nullptr, 4);
	icons[1].pixels = icons[0].pixels;
	icons[1].width = icons[0].width;
	icons[1].height = icons[0].height;

	glfwSetWindowIcon(m_Window, 2, icons);
}

void Window::setWindowResizeEventHandler(WindowResizeEventFn handler) {

	m_WindowData.windowResizeEventHandler = handler;

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {

		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.height = height;
		data.width = width;
		data.windowResizeEventHandler(WindowResizeEvent(width, height));

	});

}

void Window::setWindowMoveEventHandler(WindowMoveEventFn handler) {

	m_WindowData.windowMoveEventHandler = handler;

	glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int x, int y) {
		
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.windowMoveEventHandler(WindowMoveEvent(x, y));
		
	});

}

void Window::setWindowCloseEventHandler(WindowCloseEventFn handler) {

	m_WindowData.windowCloseEventHandler = handler;

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {

		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.windowCloseEventHandler(WindowCloseEvent());

	});

}

void Window::setMouseMoveEventHandler(MouseMoveEventFn handler) {

	m_WindowData.mouseMoveEventHandler = handler;

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {

		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.mouseMoveEventHandler(MouseMoveEvent(x, y));

	});

}

void Window::setScrollEventHandler(MouseScrollEventFn handler) {

	m_WindowData.mouseScrollEventHandler = handler;

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {

		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.mouseScrollEventHandler(MouseScrollEvent(xOffset, yOffset));

	});

}

void Window::setKeyEventHandler(KeyEventFn handler) {

	m_WindowData.keyEventHandler = handler;

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {

		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.keyEventHandler(KeyEvent(key, action));

	});

}

void Window::setJoyStickEventHandler(JoyStickEventFn handler) {

	m_WindowData.joystickEventHandler = handler;

	glfwSetJoystickCallback([](int ID, int event) {

		WindowData& data = *(WindowData*)glfwGetJoystickUserPointer(1);
		data.joystickEventHandler(JoyStickEvent(ID, event));

	});

}