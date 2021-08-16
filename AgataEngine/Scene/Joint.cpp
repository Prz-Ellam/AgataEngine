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

glm::mat4 Joint::getInverseModelTransform() const {

	return m_InverseModelTransform;

}

void Joint::setInverseModelTransform(const glm::mat4& inverseModelTransform) {

	m_InverseModelTransform = inverseModelTransform;

}

glm::mat4 Joint::getAnimateTransform() const {

	return m_AnimateTransform;

}

void Joint::setAnimateTransform(const glm::mat4& animateTransform) {

	m_AnimateTransform = animateTransform;

}

void Joint::calculateInverseModelTransform(const glm::mat4& parentModelTransform) {

	m_ModelTransform = parentModelTransform * m_LocalTransform;
	m_InverseModelTransform = glm::inverse(m_ModelTransform);
	for (Joint& joint : m_JointChildren) {
		joint.calculateInverseModelTransform(m_ModelTransform);
	}

}

void Joint::addChildJoint(Joint child) {

	m_JointChildren.push_back(child);

}