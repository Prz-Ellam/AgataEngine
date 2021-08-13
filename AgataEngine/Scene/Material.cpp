#include "Material.h"

Material::Material(const std::string& diffuseTex, const std::string& specularTex, const std::string& normalTex,
	const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
	: m_DiffuseTexture(diffuseTex, DefaultTex::DiffuseMap), m_SpecularTexture(specularTex, DefaultTex::SpecularMap),
	m_NormalMap(normalTex, DefaultTex::NormalMap), m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), 
	m_Shininess(shininess) {



}

uint32_t Material::sendDiffuseTexture(uint32_t slot) {

	m_DiffuseTexture.bind(slot);
	return slot;

}

uint32_t Material::sendSpecularTexture(uint32_t slot) {

	m_SpecularTexture.bind(slot);
	return slot;

}

uint32_t Material::sendNormalTexture(uint32_t slot) {

	m_NormalMap.bind(slot);
	return slot;

}

glm::vec3 Material::getAmbient() {

	return m_Ambient;

}

glm::vec3 Material::getDiffuse() {

	return m_Diffuse;

}

glm::vec3 Material::getSpecular() {

	return m_Specular;

}

float Material::getShininess() {

	return m_Shininess;

}

void Material::setAmbient(const glm::vec3& ambient) {

	m_Ambient = ambient;

}

void Material::setDiffuse(const glm::vec3& diffuse) {

	m_Diffuse = diffuse;

}

void Material::setSpecular(const glm::vec3& specular) {

	m_Specular = specular;

}

void Material::setShininess(float shininess) {

	m_Shininess = shininess;

}