#include "WindowResizeEvent.h"
#include "pch.h"

WindowResizeEvent::WindowResizeEvent(): m_Width(0), m_Height(0) {

}

WindowResizeEvent::WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {

}

uint32_t WindowResizeEvent::getWidth() const {

	return m_Width;

}

uint32_t WindowResizeEvent::getHeight() const {

	return m_Height;

}

std::string WindowResizeEvent::toString() const {

	std::stringstream ss;
	ss << "Window Resize Event: (" << m_Width << ", " << m_Height << ")";
	return ss.str();

}