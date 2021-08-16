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
	glm::mat4 getInverseModelTransform() const;
	void setInverseModelTransform(const glm::mat4& animateTransform);
	glm::mat4 getAnimateTransform() const;
	void setAnimateTransform(const glm::mat4& animateTransform);

	void calculateInverseModelTransform(const glm::mat4& parentModelTransform);
	void addChildJoint(Joint child);

private:
	uint32_t m_ID;
	std::string m_Name;
	std::vector<Joint> m_JointChildren;

	glm::mat4 m_AnimateTransform;
	// m_ModelTransform contiene la posicion del joint en relacion al origen, esta es la posicion original sin transformaciones
	glm::mat4 m_ModelTransform;
	// m_LocalTransform contiene la posicion del joint en relacion a su joint padre, esta es la posicion original sin transformaciones
	glm::mat4 m_LocalTransform;
	// m_InverseModelTransform contiene esta en model space, es un vector dirigido a la raiz
	glm::mat4 m_InverseModelTransform;
};

#endif