#ifndef JOINT_H
#define JOINT_H

#include <stdint.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Joint {
public:
	Joint(uint32_t id, const std::string& name, const glm::mat4& localTransform);

	glm::mat4 getModelTransform() const;
	void setModelTransform(const glm::mat4& modelTransform);
	glm::mat4 getLocalTransform() const;
	void setLocalTransform(const glm::mat4& localTransform);

private:
	uint32_t m_ID;
	std::string m_Name;
	std::vector<Joint> m_JointChildren;
	glm::mat4 m_ModelTransform;
	glm::mat4 m_LocalTransform;
	glm::mat4 m_InverseLocalTransform;
};

#endif