#ifndef WINDOW_RESIZE_EVENT_H
#define WINDOW_RESIZE_EVENT_H

#include <string>

class WindowResizeEvent {
public:
	WindowResizeEvent();
	WindowResizeEvent(uint32_t width, uint32_t height);
	virtual ~WindowResizeEvent() = default;

	uint32_t getWidth() const;
	uint32_t getHeight() const;
	std::string toString() const;
private:
	uint32_t m_Width, m_Height;
};

#endif