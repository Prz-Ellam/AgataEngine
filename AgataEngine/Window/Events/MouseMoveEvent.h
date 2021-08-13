#ifndef MOUSE_MOVE_EVENT_H
#define MOUSE_MOVE_EVENT_H

#include <string>

class MouseMoveEvent {
public:
	MouseMoveEvent();
	MouseMoveEvent(double x, double y);
	virtual ~MouseMoveEvent() = default;

	double getX() const;
	double getY() const;
	std::string toString() const;

private:
	double m_X, m_Y;
};

#endif