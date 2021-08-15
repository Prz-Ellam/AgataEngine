#include "Joint.h"

Joint::Joint(uint32_t id, const std::string& name, const glm::mat4& localTransform)
: m_ID(id), m_Name(name), m_LocalTransform(localTransform) {



}

glm::mat4 Joint::getModelTransform() const {

	return m_ModelTransform;

}

void Joint::setModelTransform(const glm::mat4& modelTransform) {

	m_ModelTransform = modelTransform;

}

glm::mat4 Joint::getLocalTransform() const {

	return m_LocalTransform;

}

void Joint::setLocalTransform(const glm::mat4& localTransform) {

	m_LocalTransform = localTransform;

}