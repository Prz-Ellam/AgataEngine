#ifndef AGATA_KEY_FRAMES_H
#define AGATA_KEY_FRAMES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class KeyFrame {
public:
	KeyFrame(float timeStamp, const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);
	glm::mat4 getTransformation() const;

	glm::vec3 getPosition() const { return m_Position; }
	glm::quat getRotation() const { return m_Rotation; }
	glm::vec3 getScale() const { return m_Scale; }

	float getTimeStamp() const;
private:
	float m_TimeStamp;
	glm::vec3 m_Position;
	glm::quat m_Rotation;
	glm::vec3 m_Scale;
};

#endif
