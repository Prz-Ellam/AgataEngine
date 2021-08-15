#include "AnimatedModel.h"
#include "Renderer.h"

void AnimatedModel::setPosition(const glm::vec3& position) {

	m_Position = position;
	m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
	m_Transformation = glm::scale(m_Transformation, m_Scale);

}

void AnimatedModel::setRotation(const glm::vec3& rotation) {

	m_Rotation = rotation;
	m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
	m_Transformation = glm::scale(m_Transformation, m_Scale);

}

glm::mat4 AnimatedModel::getTransformation() const {

	return m_Transformation;

}

void AnimatedModel::draw(std::shared_ptr<Shader> shader, Light& light, const glm::vec4& clipDistance) {

	Renderer::drawIndex(m_Mesh->getVertexArray(), m_Mesh->getIndexCount());

}