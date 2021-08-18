#ifndef AGATA_JOINT_H
#define AGATA_JOINT_H

#include <stdint.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Joint {
public:
	Joint();
	Joint(uint32_t id, const std::string& name, const glm::mat4& localTransform);

	void setAttributes(uint32_t id, const std::string& name, const glm::mat4& offsetMatrix, const glm::mat4& transformation);
	void addChildren(uint32_t count);
	Joint& getChildren(uint32_t index);
	uint32_t getChildrenCount() const {
		return m_ChildrenJoint.size();
	}

	uint32_t getID() const { return m_ID; }
	std::string getName() const { return m_Name; }


	glm::mat4 getOffsetMatrix() const { return m_OffsetMatrix; }
	glm::mat4 getTransformationMatrix() const { return m_Transformation; }

	//glm::mat4 getModelTransform() const;
	//void setModelTransform(const glm::mat4& modelTransform);
	//glm::mat4 getLocalTransform() const;
	//void setLocalTransform(const glm::mat4& localTransform);
	//glm::mat4 getInverseModelTransform() const;
	//void setInverseModelTransform(const glm::mat4& animateTransform);
	//glm::mat4 getAnimateTransform() const;
	//void setAnimateTransform(const glm::mat4& animateTransform);

	void calculateInverseModelTransform(const glm::mat4& parentModelTransform);

private:
	uint32_t m_ID;
	std::string m_Name;
	std::vector<Joint> m_ChildrenJoint;

	glm::mat4 m_OffsetMatrix;
	glm::mat4 m_Transformation;

	// m_ModelTransform contiene la posicion del joint en relacion al origen, esta es la posicion original sin transformaciones
	//glm::mat4 m_ModelTransform;
	// m_LocalTransform contiene la posicion del joint en relacion a su joint padre, esta es la posicion original sin transformaciones
	//glm::mat4 m_LocalTransform;
	// m_InverseModelTransform contiene esta en model space, es un vector dirigido a la raiz
	//glm::mat4 m_InverseModelTransform;
};

#endif