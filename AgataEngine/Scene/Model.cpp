#include "Model.h"
#include "Loader.h"
#include "Renderer.h"

//Model::Model(const std::string& filePath) : m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f, 0.0f, 0.0f), m_Scale(1.0f, 1.0f, 1.0f) {
//
//	std::vector<Vertex3D> vertices;
//	std::vector<uint32_t> indices;
//
//	Loader::loadOBJ(filePath, vertices, indices);
//
//	m_Mesh = new Mesh(vertices, indices);
//
//	m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
//	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
//	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
//	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
//	m_Transformation = glm::scale(m_Transformation, m_Scale);
//
//}

Model::Model(const std::string& filePath, const std::string& diffuseTex, const std::string& specularTex, const std::string& normalTex,
	const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, 
	const glm::vec3& ambientMaterial, const glm::vec3& diffuseMaterial, const glm::vec3& specularMaterial, 
	float shininessMaterial) 
	: m_Position(position), m_Rotation(rotation), m_Scale(scale),
	m_Material(diffuseTex, specularTex, normalTex, ambientMaterial, diffuseMaterial, specularMaterial, shininessMaterial) {

	std::vector<Vertex3D> vertices;
	std::vector<uint32_t> indices;

	Loader::loadOBJ(filePath, vertices, indices);

	m_Mesh = new Mesh(vertices, indices);

	m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
	m_Transformation = glm::scale(m_Transformation, m_Scale);

}

//Model::Model(const ModelType& modelType) : m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f, 0.0f, 0.0f), m_Scale(1.0f, 1.0f, 1.0f) {
//
//	std::vector<Vertex3D> vertices;
//	std::vector<uint32_t> indices;
//
//	Loader::loadCube(vertices, indices);
//
//	m_Mesh = new Mesh(vertices, indices);
//
//	m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
//	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
//	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
//	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
//	m_Transformation = glm::scale(m_Transformation, m_Scale);
//
//}

Model::~Model() {

	delete m_Mesh;

}

void Model::setPosition(const glm::vec3& position) {

	m_Position = position;
	m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
	m_Transformation = glm::rotate(m_Transformation,  glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
	m_Transformation = glm::rotate(m_Transformation,  glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
	m_Transformation = glm::scale(m_Transformation, m_Scale);

}

void Model::setRotation(const glm::vec3& rotation) {

	m_Rotation = rotation;
	m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
	m_Transformation = glm::scale(m_Transformation, m_Scale);

}

glm::mat4 Model::getTransformation() const {

	return m_Transformation;

}

void Model::draw() {

	Renderer::drawIndex(m_Mesh->getVertexArray(), m_Mesh->getIndexCount());

}

void Model::draw(std::shared_ptr<Shader> shader, Light& light, const glm::vec4& clipDistance) {

	shader->bind();
	shader->sendMat4("u_Model", m_Transformation);
	shader->sendMat4("u_View", Renderer::getView());
	shader->sendMat4("u_Projection", Renderer::getProjection());
	shader->sendVec3("u_CameraPos", Renderer::getPosition());
	shader->sendVec3("u_LightPos", light.getPosition());
	shader->sendVec3("u_LightColour", light.getColour());
	shader->sendInt1("u_DiffuseMap", m_Material.sendDiffuseTexture(0));
	shader->sendInt1("u_SpecularMap", m_Material.sendSpecularTexture(1));
	shader->sendInt1("u_NormalMap", m_Material.sendNormalTexture(2));
	shader->sendVec4("u_Plane", clipDistance);
	shader->sendVec4("u_Colour", m_Colour);
	shader->sendVec3("u_AmbientMaterial", m_Material.getAmbient());
	shader->sendVec3("u_DiffuseMaterial", m_Material.getDiffuse());
	shader->sendVec3("u_SpecularMaterial", m_Material.getSpecular());
	shader->sendFloat("u_ShininessMaterial", m_Material.getShininess());

	Renderer::drawIndex(m_Mesh->getVertexArray(), m_Mesh->getIndexCount());

}

ModelBuilder& ModelBuilder::ModelPath(const std::string& filePath) {

	modelFilePath = filePath;
	return *this;

}

ModelBuilder& ModelBuilder::DiffuseTexture(const std::string& filePath) {

	diffuseTexture = filePath;
	return *this;

}

ModelBuilder& ModelBuilder::SpecularTexture(const std::string& filePath) {

	specularTexture = filePath;
	return *this;

}

ModelBuilder& ModelBuilder::NormalTexture(const std::string& filePath) {

	normalTexture = filePath;
	return *this;

}

ModelBuilder& ModelBuilder::Position(const glm::vec3& position) {

	this->position = position;
	return *this;

}

ModelBuilder& ModelBuilder::Rotation(const glm::vec3& rotation) {

	this->rotation = rotation;
	return *this;

}

ModelBuilder& ModelBuilder::Scale(const glm::vec3& scale) {

	this->scale = scale;
	return *this;

}

ModelBuilder& ModelBuilder::AmbientMaterial(const glm::vec3& ambient) {

	this->ambientMaterial = ambient;
	return *this;

}

ModelBuilder& ModelBuilder::DiffuseMaterial(const glm::vec3& diffuse) {

	this->diffuseMaterial = diffuse;
	return *this;

}

ModelBuilder& ModelBuilder::SpecularMaterial(const glm::vec3& specular) {

	this->specularMaterial = specular;
	return *this;

}

ModelBuilder& ModelBuilder::ShininessMaterial(float shininess) {

	this->shininessMaterial = shininess;
	return *this;

}

Model ModelBuilder::Build() {

	return Model(modelFilePath, diffuseTexture, specularTexture, normalTexture, position, rotation, scale,
		ambientMaterial, diffuseMaterial, specularMaterial, shininessMaterial);

}

Model* ModelBuilder::BuildHeap() {

	return new Model(modelFilePath, diffuseTexture, specularTexture, normalTexture, position, rotation, scale,
		ambientMaterial, diffuseMaterial, specularMaterial, shininessMaterial);

}