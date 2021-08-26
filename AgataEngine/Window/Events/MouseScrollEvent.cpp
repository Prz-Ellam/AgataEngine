#include "MouseScrollEvent.h"
#include "pch.h"

MouseScrollEvent::MouseScrollEvent() : m_XOffset(0), m_YOffset(0) {

}

MouseScrollEvent::MouseScrollEvent(double xOffset, double yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {

}

double MouseScrollEvent::getXOffset() const {

	return m_XOffset;

}

double MouseScrollEvent::getYOffset() const {

	return m_YOffset;

}

std::string MouseScrollEvent::toString() const {

	std::stringstream ss;
	ss << "Mouse Move Event: (" << m_XOffset << ", " << m_YOffset << ")";
	return ss.str();

}