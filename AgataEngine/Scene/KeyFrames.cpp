#include "KeyFrames.h"

KeyFrame::KeyFrame(float timeStamp, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {

	m_TimeStamp = timeStamp;
	m_Position = position;
	m_Rotation = rotation;
	m_Scale = scale;

}

glm::mat4 KeyFrame::getTransformation() const {

	glm::mat4 out = glm::translate(glm::mat4(1.0f), m_Position);
	out = glm::rotate(out, glm::radians(glm::eulerAngles(m_Rotation).x), glm::vec3(1.0f, 0.0f, 0.0f));
	out = glm::rotate(out, glm::radians(glm::eulerAngles(m_Rotation).y), glm::vec3(0.0f, 1.0f, 0.0f));
	out = glm::rotate(out, glm::radians(glm::eulerAngles(m_Rotation).z), glm::vec3(0.0f, 0.0f, 1.0f));
	out = glm::scale(out, m_Scale);
	return out;

}

float KeyFrame::getTimeStamp() const {

	return m_TimeStamp;

}