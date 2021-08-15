#ifndef ANIMATED_MODEL_H
#define ANIMATED_MODEL_H

#include "Mesh.h"
#include "Material.h"
#include "Joint.h"
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

class AnimatedModel {
public:


	glm::mat4 getTransformation() const;
	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);

	Material& getMaterialRef() { return m_Material; }

	void draw(std::shared_ptr<Shader> shader, Light& light, const glm::vec4& clipDistance = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
private:
	Mesh* m_Mesh;
	Material m_Material;
	Joint m_RootJoint;
	uint32_t m_JointCount;
	glm::vec4 m_Colour;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
	glm::mat4 m_Transformation;
};

#endif