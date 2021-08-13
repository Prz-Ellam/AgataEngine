#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <string>
#include <functional>

#include "Events/WindowResizeEvent.h"
#include "Events/WindowMoveEvent.h"
#include "Events/WindowCloseEvent.h"
#include "Events/MouseMoveEvent.h"

using WindowResizeEventFn = std::function<void(WindowResizeEvent)>;
using WindowMoveEventFn = std::function<void(WindowMoveEvent)>;
using WindowCloseEventFn = std::function<void(WindowCloseEvent)>;
using MouseMoveEventFn = std::function<void(MouseMoveEvent)>;

class Window {
public:
	Window(uint32_t width, uint32_t height, const std::string& title);
	Window();
	virtual ~Window();

	void update();

	uint32_t getWidth() const;
	uint32_t getHeight() const;

	void setVSync(bool enabled);
	bool isVSync() const;

	std::string getWindowTitle();
	void setWindowTitle(const std::string& title);

	void setIcons(const std::string& filePath);

	GLFWwindow* getHandler() { return m_Window; }

	void setWindowResizeEventHandler(WindowResizeEventFn handler);
	void setWindowMoveEventHandler(WindowMoveEventFn handler);
	void setWindowCloseEventHandler(WindowCloseEventFn handler);
	void setMouseMoveEventHandler(MouseMoveEventFn handler);

private:
	GLFWwindow* m_Window;

	struct WindowData {
		uint32_t width, height;
		std::string title;
		bool vSync;
		WindowResizeEventFn windowResizeEventHandler;
		WindowMoveEventFn windowMoveEventHandler;
		WindowCloseEventFn windowCloseEventHandler;
		MouseMoveEventFn mouseMoveEventHandler;
	} m_WindowData;

};

#endif