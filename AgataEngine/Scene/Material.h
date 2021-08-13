#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture2D.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material {
public:
	Material(const std::string& diffuseTex, const std::string& specularTex, const std::string& normalTex,
		const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess);
	uint32_t sendDiffuseTexture(uint32_t slot);
	uint32_t sendSpecularTexture(uint32_t slot);
	uint32_t sendNormalTexture(uint32_t slot);

	glm::vec3 getAmbient();
	glm::vec3 getDiffuse();
	glm::vec3 getSpecular();
	float getShininess();

	void setAmbient(const glm::vec3& ambient);
	void setDiffuse(const glm::vec3& diffuse);
	void setSpecular(const glm::vec3& specular);
	void setShininess(float shininess);

private:
	Texture2D m_DiffuseTexture;
	Texture2D m_SpecularTexture;
	Texture2D m_NormalMap;
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Shininess;
};

#endif