#include "Light.h"

Light::Light(const glm::vec3& position, const glm::vec3& colour) : m_Position(position), m_Colour(colour) {

}

glm::vec3& Light::getPosition() {

	return m_Position;

}

glm::vec3& Light::getColour() {

	return m_Colour;

}

void Light::setPosition(glm::vec3 position) {

	m_Position = position;

}

void Light::setColour(glm::vec3 colour) {

	m_Colour = colour;

}

void Light::setPositionX(float x) {

	m_Position.x = x;

}

void Light::setPositionY(float y) {

	m_Position.y = y;

}

void Light::setPositionZ(float z) {

	m_Position.z = z;

}