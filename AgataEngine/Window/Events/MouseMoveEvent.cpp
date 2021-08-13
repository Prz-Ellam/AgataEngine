#include "MouseMoveEvent.h"
#include <sstream>

MouseMoveEvent::MouseMoveEvent() : m_X(0), m_Y(0) {

}

MouseMoveEvent::MouseMoveEvent(double x, double y) : m_X(x), m_Y(y) {

}

double MouseMoveEvent::getX() const {

	return m_X;

}

double MouseMoveEvent::getY() const {

	return m_Y;

}

std::string MouseMoveEvent::toString() const {

	std::stringstream ss;
	ss << "Mouse Move Event: (" << m_X << ", " << m_Y << ")";
	return ss.str();

}