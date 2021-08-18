#include "AnimatedModel.h"
#include "Renderer.h"
#include "Loader.h"

AnimatedModel::AnimatedModel(const std::string& filePath, const std::string& diffuseTex, const std::string& specularTex, 
	const std::string& normalTex, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale,
	const glm::vec3& ambientMaterial, const glm::vec3& diffuseMaterial, const glm::vec3& specularMaterial, float shininessMaterial)
	: m_Position(position), m_Rotation(rotation), m_Scale(scale),
	m_Material(diffuseTex, specularTex, normalTex, ambientMaterial, diffuseMaterial, specularMaterial, shininessMaterial) {

	std::vector<AnimVertex> vertices;
	std::vector<uint32_t> indices;

	Loader::loadCollada(filePath, vertices, indices, m_RootJoint, animations, m_GlobalInverseTransformation);
	m_JointCount = animations.size();

	m_Mesh = new Mesh(vertices, indices);

	m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
	m_Transformation = glm::rotate(m_Transformation, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
	m_Transformation = glm::scale(m_Transformation, m_Scale);

}

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

void AnimatedModel::draw(std::shared_ptr<Shader> shader, Light& light, float ts, const glm::vec4& clipDistance) {

	//ts = 0.2f;
	std::vector<glm::mat4> transformations;
	loadJointsTransforms(ts, transformations);

	shader->bind();

	for (int i = 0; i < m_JointCount; i++) {
		std::string name = "u_Joints[" + std::to_string(i) + "]";
		shader->sendMat4(name, transformations[i]);
	}

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


void AnimatedModel::loadJointsTransforms(float timeStamp, std::vector<glm::mat4>& transformations) {

	glm::mat4 identity = glm::mat4(1.0f);
	float animationTime = fmod(timeStamp, animations[0].getLength());

	transformations.resize(m_JointCount);
	findTransformations(animationTime, m_RootJoint, m_GlobalInverseTransformation, transformations);

}

void AnimatedModel::findTransformations(float animationTime, Joint& m_RootJoint, const glm::mat4& parent, 
	std::vector<glm::mat4>& transformations) {

	glm::mat4 transformationMatrix = m_RootJoint.getTransformationMatrix();
	//auto* a = glm::value_ptr(transformationMatrix);

	glm::vec3 lerpPos = lerpPosition(animationTime);
	glm::quat lerpRot = lerpRotation(animationTime);
	glm::vec3 lerpSca = lerpScale(animationTime);

	glm::mat4 out = glm::translate(glm::mat4(1.0f), lerpPos);
	out = out * glm::mat4_cast(lerpRot);
	out = glm::scale(out, lerpSca);
	out = glm::transpose(out);

	//auto* a = glm::value_ptr(out);

	glm::mat4 globalTransform = glm::transpose(glm::transpose(parent) * glm::transpose(out));
	
	transformations[m_RootJoint.getID()] = glm::transpose(m_GlobalInverseTransformation * glm::transpose(globalTransform) *
		glm::transpose(m_RootJoint.getOffsetMatrix()));
	auto* a = glm::value_ptr(transformations[m_RootJoint.getID()]);

	for (int i = 0; i < m_RootJoint.getChildrenCount(); i++) {

		findTransformations(animationTime, m_RootJoint.getChildren(i), out, transformations);

	}

}

glm::vec3 AnimatedModel::lerpPosition(float animationTime) {

	int positionIndex = 0, nextPosition;

	std::vector<KeyFrame> keyframes = animations[m_RootJoint.getID()].getKeyFrames();
	for (int i = 0; i < keyframes.size() - 1; i++) {
		if (animationTime < keyframes[i + 1].getTimeStamp()) {
			positionIndex = i;
			break;
		}
	}

	nextPosition = positionIndex + 1;
	float dt = keyframes[nextPosition].getTimeStamp() - keyframes[positionIndex].getTimeStamp();
	float factor = (animationTime - keyframes[positionIndex].getTimeStamp()) / dt;
	glm::vec3 start = keyframes[positionIndex].getPosition();
	glm::vec3 end = keyframes[nextPosition].getPosition();
	glm::vec3 delta = end - start;

	return start + factor * delta;

}

glm::quat AnimatedModel::lerpRotation(float animationTime) {

	int positionIndex = 0, nextPosition;

	std::vector<KeyFrame> keyframes = animations[m_RootJoint.getID()].getKeyFrames();
	for (int i = 0; i < keyframes.size() - 1; i++) {
		if (animationTime < keyframes[i + 1].getTimeStamp()) {
			positionIndex = i;
			break;
		}
	}

	nextPosition = positionIndex + 1;
	float dt = keyframes[nextPosition].getTimeStamp() - keyframes[positionIndex].getTimeStamp();
	float factor = (animationTime - keyframes[positionIndex].getTimeStamp()) / dt;
	glm::quat start = keyframes[positionIndex].getRotation();
	glm::quat end = keyframes[nextPosition].getRotation();
	
	//cout << a.w + a.x + a.y + a.z << endl;
	start = glm::normalize(start);
	end = glm::normalize(end);

	glm::quat result;
	float dot = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;
	float one_minus_blend = 1.0f - factor;

	if (dot < 0.0f)
	{
		result.x = start.x * one_minus_blend + factor * -end.x;
		result.y = start.y * one_minus_blend + factor * -end.y;
		result.z = start.z * one_minus_blend + factor * -end.z;
		result.w = start.w * one_minus_blend + factor * -end.w;
	}
	else
	{
		result.x = start.x * one_minus_blend + factor * end.x;
		result.y = start.y * one_minus_blend + factor * end.y;
		result.z = start.z * one_minus_blend + factor * end.z;
		result.w = start.w * one_minus_blend + factor * end.w;
	}

	return glm::normalize(result);

}

glm::vec3 AnimatedModel::lerpScale(float animationTime) {

	int positionIndex = 0, nextPosition;

	std::vector<KeyFrame> keyframes = animations[m_RootJoint.getID()].getKeyFrames();
	for (int i = 0; i < keyframes.size() - 1; i++) {
		if (animationTime < keyframes[i + 1].getTimeStamp()) {
			positionIndex = i;
			break;
		}
	}

	nextPosition = positionIndex + 1;
	float dt = keyframes[nextPosition].getTimeStamp() - keyframes[positionIndex].getTimeStamp();
	float factor = (animationTime - keyframes[positionIndex].getTimeStamp()) / dt;
	glm::vec3 start = keyframes[positionIndex].getScale();
	glm::vec3 end = keyframes[nextPosition].getScale();
	glm::vec3 delta = end - start;

	return start + factor * delta;

}

AnimatedModelBuilder& AnimatedModelBuilder::ModelPath(const std::string& filePath) {

	modelFilePath = filePath;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::DiffuseTexture(const std::string& filePath) {

	diffuseTexture = filePath;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::SpecularTexture(const std::string& filePath) {

	specularTexture = filePath;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::NormalTexture(const std::string& filePath) {

	normalTexture = filePath;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::Position(const glm::vec3& position) {

	this->position = position;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::Rotation(const glm::vec3& rotation) {

	this->rotation = rotation;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::Scale(const glm::vec3& scale) {

	this->scale = scale;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::AmbientMaterial(const glm::vec3& ambient) {

	this->ambientMaterial = ambient;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::DiffuseMaterial(const glm::vec3& diffuse) {

	this->diffuseMaterial = diffuse;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::SpecularMaterial(const glm::vec3& specular) {

	this->specularMaterial = specular;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::ShininessMaterial(float shininess) {

	this->shininessMaterial = shininess;
	return *this;

}

AnimatedModel AnimatedModelBuilder::Build() {

	return AnimatedModel(modelFilePath, diffuseTexture, specularTexture, normalTexture, position, rotation, scale,
		ambientMaterial, diffuseMaterial, specularMaterial, shininessMaterial);

}

AnimatedModel* AnimatedModelBuilder::BuildHeap() {

	return new AnimatedModel(modelFilePath, diffuseTexture, specularTexture, normalTexture, position, rotation, scale,
		ambientMaterial, diffuseMaterial, specularMaterial, shininessMaterial);

}