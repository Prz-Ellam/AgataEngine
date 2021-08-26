#ifndef MOUSE_SCROLL_EVENT_H
#define MOUSE_SCROLL_EVENT_H

#include <string>

class MouseScrollEvent {
public:
	MouseScrollEvent();
	MouseScrollEvent(double xOffset, double yOffset);
	virtual ~MouseScrollEvent() = default;

	double getXOffset() const;
	double getYOffset() const;
	std::string toString() const;

private:
	double m_XOffset, m_YOffset;
};

#endif