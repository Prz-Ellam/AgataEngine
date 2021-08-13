#ifndef WINDOW_MOVE_EVENT_H
#define WINDOW_MOVE_EVENT_H

#include <string>

class WindowMoveEvent {
public:
	WindowMoveEvent();
	WindowMoveEvent(uint32_t x, uint32_t y);
	virtual ~WindowMoveEvent() = default;

	uint32_t getX() const;
	uint32_t getY() const;
	std::string toString() const;
private:
	uint32_t m_X, m_Y;
};

#endif