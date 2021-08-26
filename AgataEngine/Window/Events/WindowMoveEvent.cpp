#include "WindowMoveEvent.h"
#include "pch.h"

WindowMoveEvent::WindowMoveEvent() : m_X(0), m_Y(0) {

}

WindowMoveEvent::WindowMoveEvent(uint32_t x, uint32_t y) : m_X(x), m_Y(y) {

}

uint32_t WindowMoveEvent::getX() const {

	return m_X;

}

uint32_t WindowMoveEvent::getY() const {

	return m_Y;

}

std::string WindowMoveEvent::toString() const {

	std::stringstream ss;
	ss << "Window Move Event: (" << m_X << ", " << m_Y << ")";
	return ss.str();

}